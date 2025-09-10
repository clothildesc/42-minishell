/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:47:11 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/10 11:47:53 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

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

static int	print_echo_arg(char *arg, t_shell *shell)
{
	int	i;

	if (arg[0] == '$' && !arg[1])
		return (ft_printf("%s", arg), 1);
	else if (!shell->env && arg[0] == '$' && arg[1] == '?' && !arg[2])
		return (ft_printf("%d", shell->status), 1);
	else if (arg[0] != '$')
		return (ft_printf("%s", arg), 1);
	else if (arg[1] && ft_isdigit(arg[1]) && arg[2])
		return (ft_printf("%s", &arg[2]), 1);
	i = 1;
	while (ft_isalnum(arg[i]))
		i++;
	if (arg[i])
		return (ft_printf("%s", &arg[i]), 1);
	return (0);
}

int	builtin_echo(char **args, t_shell *shell)
{
	bool	option;
	bool	first;
	int		i;

	i = 1;
	first = true;
	option = process_n_options(args, &i);
	while (args && args[i])
	{
		if (!first)
			ft_printf(" ");
		if (args[i][0] == '\0' || print_echo_arg(args[i], shell))
			first = false;
		i++;
	}
	if (!option)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
