/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-fati <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 05:17:08 by zel-fati          #+#    #+#             */
/*   Updated: 2026/07/12 04:20:56 by zel-fati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	scheduler_parser(char *s, int *out)
{
	if (!strcmp("fifo", s))
	{
		*out = FIFO;
		return (1);
	}
	if (!strcmp("edf", s))
	{
		*out = EDF;
		return (1);
	}
	return (0);
}

static int	check_digit(char *s, int i)
{
	int	digit;

	digit = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		digit = 1;
		i++;
	}
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (!digit)
		return (0);
	if (s[i] != '\0')
		return (0);
	return (1);
}

static int	valid_number(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '\0')
		return (0);
	return (check_digit(s, i));
}

int	validate_args(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (0);
	while (i < ac - 1)
	{
		if (!valid_number(av[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *nptr, int *out)
{
	long long	res;
	int			i;
	int			sign;

	res = 0;
	i = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - '0');
		if ((sign == 1 && res > 2147483647) || (sign == -1 && res > 2147483648))
			return (0);
		i++;
	}
	*out = (int)(res * sign);
	return (1);
}
