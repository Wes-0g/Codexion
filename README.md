*This project has been created as part of the 42 curriculum by zel-fati.*

# CODEXION

## Description

### Goal

Codexion simulates `number_of_coders` coders sitting in a circular table, each needing two shared USB dongles — one from each neighbor — to compile. There are exactly as many dongles as coders, one between each coder. The goal of the project is to implement fair, deadlock-free, starvation-free access to that shared resource under two pluggable arbitration policies, **fifo** and **edf**, using only POSIX threads, mutexes, and a priority queue.

### Overview

Each coder runs on its own thread (`routine`) and cycles through four phases: acquiring both of its dongles, compiling, debugging, and refactoring, before immediately trying to acquire again. A coder that fails to start a new compile within `time_to_burnout` milliseconds of its last one (or of the simulation's start) **burns out**, which stops the whole simulation. The simulation can also stop successfully, once every coder has compiled at least `number_of_compiles_required` times.

A dedicated **monitor thread** is solely responsible for detecting both stop conditions — it never touches dongles or the acquisition logic, it only watches every coder's deadline and compile count. Each dongle is an independent unit: its own mutex, its own cooldown timer, and its own small priority queue of pending requests, so contention on one pair of dongles never blocks coders elsewhere in the circle.

## Instructions

### Compilation

```bash
make
```

Builds with `-Wall -Wextra -Werror -pthread` and produces the `codexion` binary at the project root. Also available: `make clean`, `make fclean`, `make re`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument           | Meaning                                                    |
|--------------------|------------------------------------------------------------|
| `number_of_coders` | Number of coders, and number of dongles                    |
| `time_to_burnout`  | Max ms without starting a compile before burnout           |
| `time_to_compile`  | Compile duration in ms                                     |
| `time_to_debug`    | Debug duration in ms                                       |
| `time_to_refactor` | Refactor duration in ms                                    |
| `number_of_compiles_required` | Compiles needed per coder for a successful stop |
| `dongle_cooldown`  | Ms a released dongle stays unavailable                     |
| `scheduler`        | `fifo` or `edf`                                            |

Example:

```bash
./codexion 5 800 200 100 100 5 50 edf
```

All 8 arguments are mandatory. `validate_args`/`ft_atoi` reject non-integers, and `conf_validation` rejects negative values and a `number_of_coders` below 1 before any thread is created.

# Blocking cases handled

**Deadlock prevention (Coffman's circular-wait):** a coder never holds one dongle while waiting for the other. To acquire its two neighbor dongles, the coder must simultaneously be at the root of both dongle heaps, and both dongles must be available. `can_take_dongle` checks that both `available_at` timestamps have been reached and that the coder is the heap owner of both dongles. Only when all conditions are satisfied can the coder take both dongles and begin compiling. Therefore, a coder can never hold one dongle while waiting for another, which eliminates the hold-and-wait condition required for a circular wait and prevents this form of deadlock.

```c
static int    can_take_dongle(t_coder *coder, t_dongle *left, t_dongle *right)
{
    return (get_time_ms() >= left->available_at
        && get_time_ms() >= right->available_at
        && heap_peek(left->heap) == coder
        && heap_peek(right->heap) == coder);
}
```

- **Starvation prevention:** each dongle owns its own capacity-2 heap of pending requests. `heap_compare` orders entries by the active scheduler's key — `request_time` under `fifo`, `deadline` under `edf` — and falls through to `request_time` and finally coder `id` as deterministic tie-breakers, so no two requests are ever treated as equal by accident. A coder is only granted a dongle once it is simultaneously at the front of *both* of its dongles' heaps.
- **Startup-timing fairness:** during testing, coders that all started their first acquisition attempt at the same instant could fall into a stable pattern where one subset of coders kept winning against another. `routine` now deliberately delays every even-numbered coder (and, when `number_of_coders` is odd, the last coder as well) by one compile-plus-cooldown period before its very first attempt, which breaks that symmetry.
- **Cooldown handling:** each dongle's `available_at` field is a single sentinel value doing three jobs — `LLONG_MAX` while held, a future timestamp while cooling down, and any timestamp at or before "now" once genuinely free. `can_take_dongle` checks it directly against the current time, so a dongle can never be re-taken before `dongle_cooldown` ms have elapsed.
- **Single-coder edge case:** with one coder there is exactly one dongle, so two-handed compiling is structurally impossible. Rather than special-casing the general two-dongle algorithm, this coder is routed to its own `one_coder_routine` — it will always eventually burn out, which is the correct behavior for this input.
- **Precise burnout detection:** the monitor thread polls every coder's `deadline` under `sim_mtx` on a tight 500-microsecond interval, independently of every coder thread, so a burnout is caught and logged with only a small, bounded delay.
- **Log serialization:** `print_log`, and the monitor's own burnout print, always lock `log_mtx` around both the timestamp read and the `printf` call together, so two threads can never interleave partial output on the same line.
- **Clean shutdown on partial startup failure:** if `pthread_create` fails partway through spawning coder threads, `join_started_on_failure` signals stop and joins whichever threads did start before returning, so the program never exits while an orphaned thread is still touching memory that's about to be freed.

# Thread synchronization mechanisms

- **`d_mtx` (per dongle):** guards a single dongle's `available_at` and its private heap. `acquire_dongles` and `release_dongle` always lock a coder's two dongles in the same id-ascending order established once at the top of each function.
- **`sim_mtx` + `cond` (simulation-wide):** protects every field shared across threads that isn't dongle-specific — `stop`, `simulation_started`, and each coder's `deadline` / `last_compile_start` / `compile_count`. `wait_for_start` blocks every coder and the monitor on `cond` until `start_sim` stamps a single shared `start_ms` for all of them and broadcasts; `set_stop`/`flag_stop` use the same lock so every thread observes a stop consistently.
- **`log_mtx`:** serializes every log line written by any coder thread or the monitor thread.
- **The heap:** an array-based binary min-heap implemented from scratch (`heap_push`, `heap_pop`, `heap_peek`, `heapify_up`, `heapify_down` — no standard library priority queue). `heap_compare` is the single place the fifo/edf policy and its tie-breaking rule are defined, so both `heapify` directions and the removal logic always agree on ordering.
- **Race-prevention example:** `try_acquire` locks both of a coder's dongles before calling `can_take_dongle`, and only pops from the heaps / marks the dongles held while still holding both locks. No other coder can observe a half-updated state or believe it has acquired a dongle that's being granted to someone else in the same instant.
- **Monitor/coder coordination example:** `burnout_check` and `success_check` only ever read a coder's `deadline` and `compile_count` while holding `sim_mtx` — the exact same lock `compile()` uses when it writes those same fields after a successful acquisition. This shared-lock discipline is what closed a real race found during development, where the monitor could otherwise read a stale, pre-simulation deadline before `start_sim` had finished stamping every coder's real one, and report a false burnout.

# Resources

- POSIX Threads Programming (LLNL Tutorial) — mutexes, condition variables, thread lifecycle
- man pages: `pthread_mutex_init(3)`, `pthread_cond_wait(3)`, `pthread_cond_broadcast(3)`, `clock_gettime(3)`
- Wikipedia — Earliest Deadline First scheduling, Coffman conditions for deadlockto