/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire_dongles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 06:34:33 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/25 22:54:52 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_mtx);
	sim->stop = 1;
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->sim_mtx);
}

void	print_log(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->sim->log_mtx);
	printf("%lld %d %s\n", get_time_ms() - coder->sim->start_ms, coder->id,
		msg);
	pthread_mutex_unlock(&coder->sim->log_mtx);
}

int	flag_stop(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->sim_mtx);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->sim_mtx);
	return (stop);
}

int	acquire_dongles(t_coder *coder)
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
	while (!flag_stop(coder->sim))
	{
		pthread_mutex_lock(&lo->d_mtx);
		coder->request_time = get_time_ms();
		heap_push(lo->heap, coder);
		if (lo != hi)
		{
			pthread_mutex_lock(&hi->d_mtx);
			heap_push(hi->heap, coder);
		}
		if (get_time_ms() >= lo->available_at
			&& get_time_ms() >= hi->available_at
			&& heap_peek(lo->heap) == coder
			&& heap_peek(hi->heap) == coder)
		{
			lo->available_at = LLONG_MAX;
			heap_pop(lo->heap);
			print_log(coder, "has taken a dongle");
			if (lo != hi)
			{
				hi->available_at = LLONG_MAX;
				heap_pop(hi->heap);
				print_log(coder, "has taken a dongle");
				pthread_mutex_unlock(&hi->d_mtx);
			}
			pthread_mutex_unlock(&lo->d_mtx);
			return (1);
		}
		heap_remove(lo->heap, coder);
		if (lo != hi)
		{
			heap_remove(hi->heap, coder);
			pthread_mutex_unlock(&hi->d_mtx);
		}
		pthread_mutex_unlock(&lo->d_mtx);
		pthread_mutex_lock(&coder->sim->sim_mtx);
		if (!coder->sim->stop)
			pthread_cond_wait(&coder->sim->cond, &coder->sim->sim_mtx);
		pthread_mutex_unlock(&coder->sim->sim_mtx);
	}
	return (0);
}

void	release_dongle(t_coder *coder)
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
