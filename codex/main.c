/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 04:44:16 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/12 04:48:00 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_sim(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->sim_mtx);
	sim->start_ms = get_time_ms();
	i = 0;
	while (i < sim->conf.nb_coders)
	{
		sim->coders[i].last_compile_start = sim->start_ms;
		sim->coders[i].deadline = sim->start_ms + sim->conf.time_to_burnout;
		i++;
	}
	sim->simulation_started = 1;
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->sim_mtx);
}

static int	create_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->conf.nb_coders)
	{
		if (0 != pthread_create(&sim->coders[i].thread, NULL, routine,
				&sim->coders[i]))
			return (join_started_on_failure(sim, i));
		sim->coders_ready++;
		i++;
	}
	if (0 != pthread_create(&sim->monitor, NULL, monitor_routine, sim))
		return (join_started_on_failure(sim, i));
	return (1);
}

static int	threads_join(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->conf.nb_coders)
	{
		if (0 != pthread_join(sim->coders[i].thread, NULL))
			return (0);
		i++;
	}
	if (0 != pthread_join(sim->monitor, NULL))
		return (0);
	return (1);
}

static int	init_all(int ac, char **av, t_sim *sim)
{
	if (!arg_parser(ac, av, &sim->conf))
	{
		fprintf(stderr, "ERROR: Invalid arguments\n");
		return (0);
	}
	if (!init_sim(sim))
	{
		fprintf(stderr, "ERROR: Sim initialization failed\n");
		return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_sim	sim;

	if (!init_all(ac, av, &sim))
		return (1);
	if (!create_threads(&sim))
	{
		sim_clean_up(&sim);
		return (1);
	}
	start_sim(&sim);
	if (!threads_join(&sim))
		return (1);
	sim_clean_up(&sim);
	return (0);
}
