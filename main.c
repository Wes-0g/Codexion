/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 04:44:16 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/12 04:48:00 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->conf.nb_coders)
	{
		if (0 != pthread_create(&sim->coders[i].thread,
				NULL, routine, &sim->coders[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	threads_join(t_sim *sim, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < sim->conf.nb_coders)
	{
		if (0 != pthread_join(sim->coders[i].thread, NULL))
			return (0);
	}
	if (0 != pthread_join(monitor, NULL))
		return (0);
	return (1);
}

static int	init_all(int ac, char **av, t_sim *sim)
{
	if (!arg_parser(ac, av, &sim->conf))
	{
		fprintf(stderr, "ERROR: Invalid arguments\n");
		return (0);
	}
	if (!init_sim(sim))
	{
		fprintf(stderr, "ERROR: Sim initialization failed\n");
		return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_sim	sim;

	if (!init_all(ac, av, &sim))
		return (1);
	if (!create_coders(&sim))
		return (1);

	printf("nb_codes 	%d\n", sim.conf.nb_coders);
	printf("time_to_burnout %d\n", sim.conf.time_to_burnout);
	printf("time_to_compile %d\n", sim.conf.time_to_compile);
	printf("time_to_debug 	%d\n", sim.conf.time_to_debug);
	printf("time_to_refacto	%d\n", sim.conf.time_to_refactor);
	printf("nb_of_comp_req 	%d\n", sim.conf.nb_of_comp_req);
	printf("dongle_cooldown %d\n", sim.conf.dongle_cooldown);
	printf("scheduler 	%d\n", sim.conf.scheduler);
}
