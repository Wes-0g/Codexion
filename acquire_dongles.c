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

static int	can_take_dongle(t_coder *coder, t_dongle *lo, t_dongle *hi)
{
	return (get_time_ms() >= hi->available_at
		&& get_time_ms() >= hi->available_at
		&& heap_peek(lo->heap) == coder
		&& heap_peek(lo->heap) == coder);
}

static void	grab_dongle(t_coder *coder, t_dongle *dongle)
{
	dongle->available_at = LLONG_MAX;
	heap_pop(dongle->heap);
	print_log(coder, "has taken a dongle");
}

static void	acquire_clean(t_coder *coder, t_dongle *lo, t_dongle *hi)
{
	heap_remove(lo->heap, coder);
	if (lo != hi)
	{
		heap_remove(hi->heap, coder);
		pthread_mutex_unlock(&hi->d_mtx);
	}
	pthread_mutex_unlock(&lo->d_mtx);
}

static int	try_acquire(t_coder *coder, t_dongle *lo, t_dongle *hi)
{
	pthread_mutex_lock(&lo->d_mtx);
	heap_push(lo->heap, coder);
	if (lo != hi)
	{
		pthread_mutex_lock(&hi->d_mtx);
		heap_push(hi->heap, coder);
	}
	if (can_take_dongle(coder, lo, hi))
	{
		grab_dongle(coder, lo);
		if (lo != hi)
		{
			grab_dongle(coder, hi);
			pthread_mutex_unlock(&hi->d_mtx);
		}
		pthread_mutex_unlock(&lo->d_mtx);
		return (1);
	}
	return (0);
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
	coder->request_time = get_time_ms();
	while (!flag_stop(coder->sim))
	{
		if (try_acquire(coder, lo, hi))
			return (1);
		acquire_clean(coder, lo, hi);
		pthread_mutex_lock(&coder->sim->sim_mtx);
		if (!coder->sim->stop)
			pthread_cond_wait(&coder->sim->cond, &coder->sim->sim_mtx);
		pthread_mutex_unlock(&coder->sim->sim_mtx);
	}
	return (0);
}
