/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_coder.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 02:34:52 by zel-fati          #+#    #+#             */
/*   Updated: 2026/08/06 02:35:16 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	one_coder_routine(t_coder *coder, t_dongle *left, t_dongle *right)
{
	pthread_mutex_lock(&left->d_mtx);
	coder->request_time = get_time_ms();
	heap_push(left->heap, coder);
	if (left != right)
	{
		pthread_mutex_lock(&right->d_mtx);
		heap_push(right->heap, coder);
	}
	if ((heap_peek(left->heap) == coder && get_time_ms() >= left->available_at)
		|| (heap_peek(right->heap) == coder
			&& get_time_ms() >= left->available_at))
	{
		left->available_at = LLONG_MAX;
		heap_pop(left->heap);
		print_log(coder, "has taken a dongle");
		if (left != right)
		{
			left->available_at = LLONG_MAX;
			heap_pop(right->heap);
			print_log(coder, "has taken a dongle");
			pthread_mutex_unlock(&right->d_mtx);
		}
		pthread_mutex_unlock(&left->d_mtx);
	}
}
