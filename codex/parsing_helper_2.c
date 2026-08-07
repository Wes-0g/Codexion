/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 02:56:19 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/12 04:51:24 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	conf_validation(t_config *conf)
{
	if (conf->nb_coders <= 0)
		return (0);
	if (conf->time_to_burnout < 0)
		return (0);
	if (conf->time_to_compile < 0)
		return (0);
	if (conf->time_to_debug < 0)
		return (0);
	if (conf->time_to_refactor < 0)
		return (0);
	if (conf->nb_of_comp_req < 0)
		return (0);
	if (conf->dongle_cooldown < 0)
		return (0);
	return (1);
}

int	arg_parser(int ac, char **av, t_config *conf)
{
	if (!validate_args(ac, av))
		return (0);
	if (!ft_atoi(av[1], &conf->nb_coders))
		return (0);
	if (!ft_atoi(av[2], &conf->time_to_burnout))
		return (0);
	if (!ft_atoi(av[3], &conf->time_to_compile))
		return (0);
	if (!ft_atoi(av[4], &conf->time_to_debug))
		return (0);
	if (!ft_atoi(av[5], &conf->time_to_refactor))
		return (0);
	if (!ft_atoi(av[6], &conf->nb_of_comp_req))
		return (0);
	if (!ft_atoi(av[7], &conf->dongle_cooldown))
		return (0);
	if (!scheduler_parser(av[8], &conf->scheduler))
		return (0);
	if (!conf_validation(conf))
		return (0);
	return (1);
}
