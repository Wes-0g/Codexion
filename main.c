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

int	main(int ac, char **av)
{
	t_config	conf;

	if (!arg_parser(ac, av, &conf))
	{
		write(2, "ERROR\n", 6);
		return (1);
	}
	printf("nb_codes 	%d\n", conf.nb_coders);
	printf("time_to_burnout %d\n", conf.time_to_burnout);
	printf("time_to_compile %d\n", conf.time_to_compile);
	printf("time_to_debug 	%d\n", conf.time_to_debug);
	printf("time_to_refacto	%d\n", conf.time_to_refactor);
	printf("nb_of_comp_req 	%d\n", conf.nb_of_comp_req);
	printf("dongle_cooldown %d\n", conf.dongle_cooldown);
	printf("scheduler 	%d\n", conf.scheduler);
}
