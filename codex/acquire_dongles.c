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

static void	enqueue_coder(t_coder *coder, t_dongle *left, t_dongle *right)
{
	coder->request_time = get_time_ms();
	pthread_mutex_lock(&left->d_mtx);
	pthread_mutex_lock(&right->d_mtx);
	heap_push(left->heap, coder);
	heap_push(right->heap, coder);
	pthread_mutex_unlock(&right->d_mtx);
	pthread_mutex_unlock(&left->d_mtx);
}

static int	can_take_dongle(t_coder *coder, t_dongle *left, t_dongle *right)
{
	return (get_time_ms() >= left->available_at
		&& get_time_ms() >= right->available_at
		&& heap_peek(left->heap) == coder
		&& heap_peek(right->heap) == coder);
}

static void	grab_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
	left->available_at = LLONG_MAX;
	right->available_at = LLONG_MAX;
	heap_pop(left->heap);
	heap_pop(right->heap);
	print_log(coder, "has taken a dongle");
	print_log(coder, "has taken a dongle");
}

static int	try_acquire(t_coder *coder, t_dongle *left, t_dongle *right)
{
	pthread_mutex_lock(&left->d_mtx);
	pthread_mutex_lock(&right->d_mtx);
	if (can_take_dongle(coder, left, right))
	{
		grab_dongles(coder, left, right);
		pthread_mutex_unlock(&right->d_mtx);
		pthread_mutex_unlock(&left->d_mtx);
		return (1);
	}
	pthread_mutex_unlock(&right->d_mtx);
	pthread_mutex_unlock(&left->d_mtx);
	return (0);
}

int	acquire_dongles(t_coder *coder)
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
	if (coder->sim->conf.nb_coders == 1)
		return (one_coder_routine(coder, left, right));
	enqueue_coder(coder, left, right);
	while (!flag_stop(coder->sim))
	{
		if (try_acquire(coder, left, right))
			return (1);
		usleep(300);
	}
	return (0);
}
