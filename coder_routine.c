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
	t_dongle	*left;
	t_dongle	*right;

	if (coder->left->id > coder->right->id)
	{
		left = coder->right;
		right = coder->left;
	}
	else
	{
		left = coder->left;
		right = coder->right;
	}
	pthread_mutex_lock(&left->d_mtx);
	pthread_mutex_lock(&right->d_mtx);
	left->available_at = get_time_ms() + coder->sim->conf.dongle_cooldown;
	right->available_at = get_time_ms() + coder->sim->conf.dongle_cooldown;
	pthread_mutex_unlock(&left->d_mtx);
	pthread_mutex_unlock(&right->d_mtx);
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
	print_log(coder, "is compiling", 0);
	custom_sleep(sim, sim->conf.time_to_compile);
	return (!flag_stop(sim));
}

static int	debug(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (flag_stop(sim))
		return (0);
	print_log(coder, "is debugging", 0);
	custom_sleep(sim, sim->conf.time_to_debug);
	return (!flag_stop(sim));
}

static int	refactor(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (flag_stop(sim))
		return (0);
	print_log(coder, "is refactoring", 0);
	custom_sleep(sim, sim->conf.time_to_refactor);
	return (!flag_stop(sim));
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
	if (coder->id % 2 == 0)
		custom_sleep(sim, sim->conf.time_to_compile
			+ sim->conf.dongle_cooldown);
	else if (coder->id == sim->conf.nb_coders && sim->conf.nb_coders % 2 == 1)
			custom_sleep(sim, sim->conf.time_to_compile + sim->conf.dongle_cooldown);
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
