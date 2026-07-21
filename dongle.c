/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 06:52:32 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/20 01:11:07 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	dongle_free(t_sim *sim, int i)
{
	while (i >= 0)
	{
		heap_destroy(sim->dongles[i].heap);
		if (sim->dongles[i].mutex_init)
			pthread_mutex_destroy(&sim->dongles[i].d_mtx);
		i--;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->conf.nb_coders);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->conf.nb_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].available_at = 0;
		sim->dongles[i].mutex_init = 0;
		sim->dongles[i].heap = create_heap(2, sim->conf);
		if (!sim->dongles[i].heap)
			return (dongle_free(sim, i), 0);
		if (0 != pthread_mutex_init(&sim->dongles[i].d_mtx, NULL))
			return (dongle_free(sim, i), 0);
		sim->dongles[i].mutex_init = 1;
		i++;
	}
	return (1);
}

int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->conf.nb_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->conf.nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_counter = 0;
		sim->coders[i].request_time = 
		sim->coders[i].deadline = sim->coders[i].last_compile_start
			+ sim->conf.time_to_burnout;
		sim->coders[i].right = sim->dongles[(i + 1) % sim->conf.nb_coders];
		sim->coders[i].left = sim->dongles[(i - 1 + sim->conf.nb_coders) % sim->conf.nb_coders];
	}
}

int	init_sim(t_sim *sim)
{
	sim->start_ms = 0;
	sim->coders_ready = 0;
	sim->simulation_started = 0;
	sim->stop = 0;
	if (0 != pthread_mutex_init(&sim->log_mtx, NULL))
		return (0);
	if (0 != pthread_mutex_init(&sim->sim_mtx, NULL))
		return (0); // Destroy mutex
	if (0 != pthread_cond_init(&sim->cond, NULL))
		return (0); // Destroy all other mutex
	if (!init_dongles(sim))
		return (0);

	// init_coders
	return (1);
}
