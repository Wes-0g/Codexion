/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 03:22:20 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/17 06:35:20 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*create_heap(int capacity)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->capacity = capacity;
	heap->size = 0;
	heap->coders = malloc(sizeof(t_coder *) * capacity);
	if (!heap->coders)
		return (NULL);
	return (heap);
}

void	swap(t_coder **a, t_coder **b)
{
	t_coder	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	heapify_up(t_heap *heap, int i)
{
	if (i && heap->coders[(i - 1) / 2]->request_time > heap->coders[i]->request_time)
	{
		swap(&heap->coders[(i - 1) / 2], &heap->coders[i]);
		heapify_up(heap, (i - 1) / 2);
	}
}

void	heapify_down(t_heap *heap, int i)
{
	int	smallest;
	int	left_c;
	int	right_c;

	smallest = i;
	left_c = 2 * i + 1;
	right_c = 2 * i + 2;
	if (left_c < heap->size && heap->coders[left_c]->request_time < heap->coders[smallest]->request_time)
		smallest = left_c;
	if (right_c < heap->size && heap->coders[right_c]->request_time < heap->coders[smallest]->request_time)
		smallest = right_c;
	if (smallest != i)
	{
		swap(&heap->coders[i], &heap->coders[smallest]);
		heapify_down(heap, smallest);
	}
}

int	heap_push(t_heap *heap, t_coder *coder)
{
	if (heap->size == heap->capacity)
		return (0);
	heap->coders[heap->size++] = coder;
	heapify_up(heap, heap->size - 1);
	return (1);
}

t_coder	*heap_pop(t_heap *heap)
{
	t_coder	*coder;

	if (!heap->size)
		return (NULL);
	coder = heap->coders[0];
	heap->coders[0] = heap->coders[--heap->size];
	heapify_down(heap, 0);
	return (coder);
}

t_coder	*heap_peek(t_heap *heap)
{
	if (!heap->size)
		return (NULL);
	return (heap->coders[0]);
}

void	heap_destroy(t_heap *heap)
{
	if (heap)
	{
		free(heap->coders);
		free(heap);
	}
}

void	*func(void *arg)
{
	printf("CREATING THREAD --- %s\n", (char *)arg);
	return (NULL);
}

int	main(void)
{
	t_heap	*heap;
	t_coder	coder1;
	t_coder	coder2;
	t_coder	*c;
	t_coder	*c1;

	heap = create_heap(2);
	coder1.id = 1;
	coder1.request_time = 1000;
	if (0 != pthread_create(&coder1.thread, NULL, func, (void *)"1"))
		perror("ERROR creating thread");
	pthread_join(coder1.thread, NULL);

	coder2.id = 2;
	coder2.request_time = 100000;
	if (0 != pthread_create(&coder2.thread, NULL, func, (void *)"2"))
		perror("ERROR creating thread");
	pthread_join(coder2.thread, NULL);

	heap_push(heap, &coder1);
	heap_push(heap, &coder2);
	c = heap_pop(heap);
	c1 = heap_pop(heap);
	printf("%d\n", c->id);
	printf("%d\n", c1->id);
	heap_destroy(heap);
}
