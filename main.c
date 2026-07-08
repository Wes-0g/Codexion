/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 04:44:16 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/08 02:43:43 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void	*func(void *arg)
{
	printf("thread created\n");
	return NULL;
}

int	main(int ac, char **av)
{
	printf("program entry\n");
	
	/*int i = 0;

	printf("%d\n", ac);

	while (i < ac)
	{
		printf("%s\n", av[i]);
		i++;
	}*/

	pthread_t thread1;	

	pthread_create(&thread1, NULL, func, NULL);

	pthread_join(thread1, NULL);

	return (0);
}
