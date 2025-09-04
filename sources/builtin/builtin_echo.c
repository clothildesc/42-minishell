/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:47:11 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/04 15:20:47 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_n_option(char *arg)
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
				return (false);
			i++;
		}
		return (true);
	}
	return (false);
}

static bool	process_n_options(char **args, int *i)
{
	bool	option;

	option = false;
	while (args && args[*i] && is_n_option(args[*i]))
	{
		option = true;
		(*i)++;
	}
	return (option);
}

static void	write_echo(char *arg)
{
	static bool	first = true;

	if (!first)
		ft_printf(" ");
	if (arg[0] == '$' && arg[1] && ft_isdigit(arg[1]) \
		&& arg[2])
	{
		ft_printf("%s", &arg[2]);
		first = false;
	}
	else if (arg[0] != '$')
	{
		ft_printf("%s", arg);
		first = false;
	}
}

int	builtin_echo(char **args)
{
	bool	option;
	int		i;

	i = 1;
	option = process_n_options(args, &i);
	while (args && args[i])
	{
		write_echo(args[i]);
		i++;
	}
	if (!option)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
