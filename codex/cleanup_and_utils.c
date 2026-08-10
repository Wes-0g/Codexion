/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:29:06 by zel-fati          #+#    #+#             */
/*   Updated: 2026/08/07 16:29:57 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	join_started_on_failure(t_sim *sim, int i)
{
	set_stop(sim);
	while (--i >= 0)
	{
		pthread_join(sim->coders[i].thread, NULL);
	}
	return (0);
}

void	sim_clean_up(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->conf.nb_coders)
	{
		if (sim->dongles[i].mutex_init)
			pthread_mutex_destroy(&sim->dongles[i].d_mtx);
		heap_destroy(sim->dongles[i].heap);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
	free(sim->coders);
	sim->coders = NULL;
	if (sim->cond_init)
		pthread_cond_destroy(&sim->cond);
	if (sim->sim_mtx_init)
		pthread_mutex_destroy(&sim->sim_mtx);
	if (sim->log_mtx_init)
		pthread_mutex_destroy(&sim->log_mtx);
}

long long	get_time_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	return ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000));
}
