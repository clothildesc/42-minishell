/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:47:11 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/18 14:32:19 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_option(t_arg *arg)
{
	int	i;

	i = 2;
	if (!arg)
		return (0);
	if (ft_strncmp(arg->arg, "-n", 2) == 0)
	{
		while (arg->arg[i])
		{
			if (arg->arg[i] != 'n')
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

int	builtin_echo(t_arg *args)
{
	t_arg	*current;
	bool	option;
	bool	first;

	option = false;
	first = true;
	current = args;
	while (is_n_option(current))
	{
		if (is_n_option(current))
			option = true;
		current = current->next;
	}
	while (current)
	{
		if (!first)
			ft_printf(" ");
		ft_printf("%s", current->arg);
		first = false;
		current = current->next;
	}
	if (!args || !option)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
