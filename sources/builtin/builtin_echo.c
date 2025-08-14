/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:47:11 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/14 17:49:41 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_option(t_arg *arg)
{
	int	i;

	i = 2;
	if (ft_strncmp(arg->arg, "-n", 2))
	{
		while (arg->arg[i])
		{
			if (arg->arg[i] != 'n')
				return (0);
			i++;
		}
	}
	return (1);
}

int	builtin_echo(t_arg *args)
{
	t_arg	*current;
	bool	option;

	option = true;
	current = args;
	while (is_n_option(current))
		current = current->next;
	while (current)
	{
		ft_printf("%s", current->arg);
		current = current->next;
	}
	if (!option)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
