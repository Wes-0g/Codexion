/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 06:52:32 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/22 06:51:36 by zel-fati         ###   ########.fr       */
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

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles_init = 0;
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
	sim->dongles_init = 1;
	return (1);
}

static int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders_init = 0;
	sim->coders = malloc(sizeof(t_coder) * sim->conf.nb_coders);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->conf.nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].request_time = 0;
		sim->coders[i].left = &sim->dongles[i];
		if (i == sim->conf.nb_coders - 1)
			sim->coders[i].right = &sim->dongles[0];
		else
			sim->coders[i].right = &sim->dongles[i + 1];
		sim->coders[i].sim = sim;
		i++;
	}
	sim->coders_init = 1;
	return (1);
}

static void	init_sim_error(t_sim *sim)
{
	if (sim->log_mtx_init)
		pthread_mutex_destroy(&sim->log_mtx);
	if (sim->sim_mtx_init)
		pthread_mutex_destroy(&sim->sim_mtx);
	if (sim->cond_init)
		pthread_cond_destroy(&sim->cond);
	if (sim->dongles_init)
		dongle_free(sim, sim->conf.nb_coders - 1);
	if (sim->coders_init)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
}

int	init_sim(t_sim *sim)
{
	sim->start_ms = 0;
	sim->coders_ready = 0;
	sim->simulation_started = 0;
	sim->stop = 0;
	sim->log_mtx_init = 0;
	sim->sim_mtx_init = 0;
	sim->cond_init = 0;
	if (0 != pthread_mutex_init(&sim->log_mtx, NULL))
		return (0);
	sim->log_mtx_init = 1;
	if (0 != pthread_mutex_init(&sim->sim_mtx, NULL))
		return (init_sim_error(sim), 0);
	sim->sim_mtx_init = 1;
	if (0 != pthread_cond_init(&sim->cond, NULL))
		return (init_sim_error(sim), 0);
	sim->cond_init = 1;
	if (!init_dongles(sim))
		return (init_sim_error(sim), 0);
	if (!init_coders(sim))
		return (init_sim_error(sim), 0);
	return (1);
}
