/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 04:20:05 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/19 04:20:18 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*create_heap(int capacity, t_config conf)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->capacity = capacity;
	heap->size = 0;
	heap->conf = conf;
	heap->coders = malloc(sizeof(t_coder *) * capacity);
	if (!heap->coders)
		return (NULL);
	return (heap);
}

void	heap_destroy(t_heap *heap)
{
	if (heap)
	{
		free(heap->coders);
		free(heap);
	}
}

long long	get_time_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000));
}
