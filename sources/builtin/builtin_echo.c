/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:47:11 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/12 14:31:06 by cscache          ###   ########.fr       */
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

int	ft_print_no_quote(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != '\'')
			write(1, &arg[i], 1);
		i++;
	}
	return (1);
}

// int	handle_quoted_str(char *arg, t_shell *shell)
// {
// 	int		i;

// 	i = 0;
// 	while (arg[i])
// 	{
// 		if (arg[i] != '\'' && arg[i] != '$')
// 			write(1, &arg[i], 1);
// 		else
// 		{
// 			if (arg[i] == '\'')
// 				return (ft_print_no_quote(&arg[i]));
// 			i++;
// 			return (print_echo_arg(&arg[i], shell));
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// int	print_echo_arg(char *arg, t_shell *shell)
// {
// 	int	i;

// 	printf("arg = %s\n", arg);
// 	if (arg[0] != '$')
// 		return (handle_quoted_str(arg, shell), 1);
// 	if (!arg[1])
// 		return (ft_printf("%s", arg), 1);
// 	if (!shell->env && arg[1] == '?' && !arg[2])
// 		return (ft_printf("%d", shell->status), 1);
// 	if (ft_isdigit(arg[1]) && arg[2])
// 		return (ft_printf("%s", &arg[2]), 1);
// 	i = 1;
// 	while (ft_isalnum(arg[i]))
// 		i++;
// 	if (arg[i])
// 		return (ft_printf("%s", &arg[i]), 1);
// 	return (0);
// }


int	builtin_echo(char **args, t_shell *shell)
{
	bool	option;
	bool	first;
	int		i;

	(void)shell;
	i = 1;
	first = true;
	option = process_n_options(args, &i);
	while (args && args[i])
	{
		if (!first)
			ft_printf(" ");
		ft_printf("%s",args[i]);
		//if (args[i][0] == '\0')
		first = false;
		i++;
	}
	if (!option)
		ft_printf("\n");
	return (EXIT_SUCCESS);
}
