/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 23:59:14 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/28 03:18:31 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	burnout_check(t_sim *sim)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&sim->sim_mtx);
	while (i < sim->conf.nb_coders)
	{
		if (get_time_ms() >= sim->coders[i].deadline)
		{
			pthread_mutex_unlock(&sim->sim_mtx);
			set_stop(sim);
			print_log(&sim->coders[i], "burned out");
			return ;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->sim_mtx);
}

static void	success_check(t_sim *sim)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&sim->sim_mtx);
	while (i < sim->conf.nb_coders)
	{
		if (sim->coders[i].compile_count < sim->conf.nb_of_comp_req)
		{
			pthread_mutex_unlock(&sim->sim_mtx);
			return ;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->sim_mtx);
	set_stop(sim);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!flag_stop(sim))
	{
		burnout_check(sim);
		if (flag_stop(sim))
			return (NULL);
		success_check(sim);
		if (flag_stop(sim))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
