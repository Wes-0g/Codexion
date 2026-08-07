/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 03:22:20 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/19 04:26:33 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heapify_up(t_heap *heap, int i, int scheduler)
{
	if (i && heap_compare(heap->coders[(i - 1) / 2], heap->coders[i],
			scheduler) > 0)
	{
		swap(&heap->coders[(i - 1) / 2], &heap->coders[i]);
		heapify_up(heap, (i - 1) / 2, scheduler);
	}
}

void	heapify_down(t_heap *heap, int i, int scheduler)
{
	int	smallest;
	int	left_c;
	int	right_c;

	smallest = i;
	left_c = 2 * i + 1;
	right_c = 2 * i + 2;
	if (left_c < heap->size && heap_compare(heap->coders[left_c],
			heap->coders[smallest], scheduler) < 0)
		smallest = left_c;
	if (right_c < heap->size && heap_compare(heap->coders[right_c],
			heap->coders[smallest], scheduler) < 0)
		smallest = right_c;
	if (smallest != i)
	{
		swap(&heap->coders[i], &heap->coders[smallest]);
		heapify_down(heap, smallest, scheduler);
	}
}

int	heap_push(t_heap *heap, t_coder *coder)
{
	if (heap->size == heap->capacity)
		return (0);
	heap->coders[heap->size++] = coder;
	heapify_up(heap, heap->size - 1, heap->conf.scheduler);
	return (1);
}

t_coder	*heap_pop(t_heap *heap)
{
	t_coder	*coder;

	if (!heap->size)
		return (NULL);
	coder = heap->coders[0];
	heap->coders[0] = heap->coders[--heap->size];
	heapify_down(heap, 0, heap->conf.scheduler);
	return (coder);
}

t_coder	*heap_peek(t_heap *heap)
{
	if (!heap->size)
		return (NULL);
	return (heap->coders[0]);
}
