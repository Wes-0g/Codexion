/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 04:18:06 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/19 04:18:34 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_coder **a, t_coder **b)
{
	t_coder	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

long long	get_priority(t_coder *coder, int scheduler)
{
	if (scheduler == FIFO)
		return (coder->request_time);
	return (coder->deadline);
}

int	heap_compare(t_coder *a, t_coder *b, int scheduler)
{
	long long	a_p;
	long long	b_p;

	a_p = get_priority(a, scheduler);
	b_p = get_priority(b, scheduler);
	if (a_p != b_p)
	{
		if (a_p < b_p)
			return (-1);
		return (1);
	}
	if (a->id < b->id)
		return (-1);
	return (1);
}

void	heap_remove(t_heap *heap, t_coder *coder)
{
	int	i;
	int	parent;

	i = 0;
	while (i < heap->size && heap->coders[i] != coder)
		i++;
	if (i == heap->size)
		return ;
	heap->size--;
	if (i == heap->size)
		return ;
	heap->coders[i] = heap->coders[heap->size];
	parent = (i - 1) / 2;
	if (i && heap_compare(heap->coders[i], heap->coders[parent],
			heap->conf.scheduler) < 0)
		heapify_up(heap, i, heap->conf.scheduler);
	else
		heapify_down(heap, i, heap->conf.scheduler);
}
