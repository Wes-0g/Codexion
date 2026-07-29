/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 17:13:50 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/27 17:13:54 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	release_dongle(t_coder *coder)
{
	t_dongle	*lo;
	t_dongle	*hi;

	if (coder->left->id < coder->right->id)
	{
		lo = coder->left;
		hi = coder->right;
	}
	else
	{
		lo = coder->right;
		hi = coder->left;
	}
	pthread_mutex_lock(&lo->d_mtx);
	lo->available_at = get_time_ms() + coder->sim->conf.dongle_cooldown;
	pthread_mutex_unlock(&lo->d_mtx);
	if (hi != lo)
	{
		pthread_mutex_lock(&hi->d_mtx);
		hi->available_at = get_time_ms() + coder->sim->conf.dongle_cooldown;
		pthread_mutex_unlock(&hi->d_mtx);
	}
	pthread_cond_broadcast(&coder->sim->cond);
}

static int	compile(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (flag_stop(sim))
		return (0);
	pthread_mutex_lock(&sim->sim_mtx);
	coder->last_compile_start = get_time_ms();
	coder->deadline = coder->last_compile_start + sim->conf.time_to_burnout;
	coder->compile_count++;
	pthread_mutex_unlock(&sim->sim_mtx);
	print_log(coder, "is compiling");
	custom_sleep(sim, sim->conf.time_to_compile);
	return (!flag_stop(sim));
}

static int	debug(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (flag_stop(sim))
		return (0);
	print_log(coder, "is debugging");
	custom_sleep(sim, sim->conf.time_to_debug);
	return (!flag_stop(sim));
}

static int	refactor(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (flag_stop(sim))
		return (0);
	print_log(coder, "is refactoring");
	custom_sleep(sim, sim->conf.time_to_refactor);
	return (!flag_stop(sim));
}

static void	wait_for_start(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	sim->coders_ready++;
	if (sim->coders_ready == sim->conf.nb_coders)
		pthread_cond_broadcast(&sim->cond);
	while (!sim->simulation_started && !sim->stop)
		pthread_cond_wait(&sim->cond, &sim->sim_mtx);
	pthread_mutex_unlock(&sim->sim_mtx);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	wait_for_start(sim);
	if (flag_stop(sim))
		return (NULL);
	while (!flag_stop(sim))
	{
		if (!acquire_dongles(coder))
			break ;
		if (!compile(coder))
			break ;
		release_dongle(coder);
		if (!debug(coder))
			break ;
		if (!refactor(coder))
			break ;
	}
	return (NULL);
}
