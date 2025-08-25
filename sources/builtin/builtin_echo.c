/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:47:11 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/25 17:47:41 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_option(char *arg)
{
	int	i;

	i = 2;
	if (!arg)
		return (0);
	if (ft_strncmp(arg, "-n", 2) == 0)
	{
		while (arg[i])
		{
			if (arg[i] != 'n')
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

int	builtin_echo(char **args, t_shell *shell)
{
	bool	option;
	bool	first;
	int		i;

	option = false;
	first = true;
	i = 1;
	while (args && args[i] && is_n_option(args[i]))
	{
		option = true;
		i++;
	}
	while (args && args[i])
	{
		if (!first)
			ft_printf(" ");
		if (ft_strcmp(" $?", args[i]))
			ft_printf("%d", shell->exit_status);
		else
			ft_printf("%s", args[i]);
		first = false;
		i++;
	}
	if (!option)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
