/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 03:51:41 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/12 04:46:43 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

# define FIFO 0
# define EDF 1

typedef struct s_config
{
	int	nb_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	nb_of_comp_req;
	int	dongle_cooldown;
	int	scheduler;
}		t_config;

int		validate_args(int ac, char **av);
int		ft_atoi(char *nptr, int *out);
int		scheduler_parser(char *s, int *out);
int		arg_parser(int ac, char **av, t_config *conf);

#endif
