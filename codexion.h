/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 03:51:41 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/12 04:46:43 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

# define FIFO 0
# define EDF 1

typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;
typedef struct s_heap t_heap;
typedef struct s_sim t_sim;

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
}		t_config;

struct s_sim
{
	t_config args;
	t_coder *coders;
	t_dongle *dongles;
};

struct s_dongle
{
	pthread_mutex_t mtx;
	pthread_cond_t cond;
	int id;
	long long available_at;
	t_heap *heap;
};

struct s_coder
{
	pthread_t thread;
	int id;
	t_dongle *left;
	t_dongle *right;
	t_sim *sim;
};

struct s_heap
{

};

int		validate_args(int ac, char **av);
int		ft_atoi(char *nptr, int *out);
int		scheduler_parser(char *s, int *out);
int		arg_parser(int ac, char **av, t_config *conf);

#endif
