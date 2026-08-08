/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 22:59:11 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/28 04:15:54 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->sim_mtx);
}

int	flag_stop(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->sim_mtx);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->sim_mtx);
	return (stop);
}

void	print_log(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->sim->log_mtx);
	if (!flag_stop(coder->sim))
		printf("%lld %d %s\n", get_time_ms() - coder->sim->start_ms, coder->id,
			msg);
	pthread_mutex_unlock(&coder->sim->log_mtx);
}

void	custom_sleep(t_sim *sim, long long time)
{
	long long	st;

	st = get_time_ms();
	while (!flag_stop(sim))
	{
		if (get_time_ms() - st >= time)
			break ;
		usleep(500);
	}
}

void	wait_for_start(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	while (!sim->simulation_started && !sim->stop)
		pthread_cond_wait(&sim->cond, &sim->sim_mtx);
	pthread_mutex_unlock(&sim->sim_mtx);
}
