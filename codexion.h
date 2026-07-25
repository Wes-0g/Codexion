/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 03:51:41 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/23 06:11:09 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
#include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

# define FIFO 0
# define EDF 1

typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct s_heap	t_heap;
typedef struct s_sim	t_sim;

typedef struct s_config
{
	int	nb_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	nb_of_comp_req;
	int	dongle_cooldown;
	int	scheduler;
}	t_config;

struct	s_sim
{
	long long	start_ms;
	int	stop;
	int	coders_ready;
	int	simulation_started;

	t_config	conf;
	pthread_mutex_t		log_mtx;
	pthread_mutex_t sim_mtx;
	pthread_cond_t	cond;

	int	dongles_init;
	int	coders_init;
	int	log_mtx_init;
	int	sim_mtx_init;
	int	cond_init;

	t_coder		*coders;
	t_dongle	*dongles;
};

struct	s_dongle
{
	int	id;
	int	mutex_init;
	long long	available_at;
	pthread_mutex_t	d_mtx;
	t_heap	*heap;
};

struct	s_coder
{
	pthread_t			thread;
	t_dongle			*left;
	t_dongle			*right;
	t_sim				*sim;
	int	id;
	long long	request_time;
	long long	deadline;
	long long	last_compile_start;
	int	compile_count;
	int in_heap;
};

struct	s_heap
{
	t_config	conf;
	t_coder	**coders;
	int	size;
	int	capacity;

};

int	validate_args(int ac, char **av);
int	ft_atoi(char *nptr, int *out);
int	scheduler_parser(char *s, int *out);
int	arg_parser(int ac, char **av, t_config *conf);

void	swap(t_coder **a, t_coder **b);
int	heap_push(t_heap *heap, t_coder *coder);
t_coder	*heap_pop(t_heap *heap);
t_coder	*heap_peek(t_heap *heap);
void	heap_remove(t_heap *heap, t_coder *coder);
void	heapify_up(t_heap *heap, int i, int scheduler);
void	heapify_down(t_heap *heap, int i, int scheduler);
int	heap_compare(t_coder *a, t_coder *b, int scheduler);
t_heap	*create_heap(int capacity, t_config conf);
void	heap_destroy(t_heap *heap);

long long	get_time_ms(void);

int	init_sim(t_sim *sim);

#endif
