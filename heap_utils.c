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

static long long	get_priority(t_coder *coder, int scheduler)
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
	if (a->request_time != b->request_time)
	{
		if (a->request_time < b->request_time)
			return (-1);
		return (1);
	}
	if (a->id < b->id)
		return (-1);
	return (1);
}
