/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:46:59 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/05 10:13:45 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_exit(t_shell *shell, char **args, int saved_in, int saved_out)
{
	int	nb_args;
	int	n;

	nb_args = 0;
	while (args[nb_args])
		nb_args++;
	ft_putendl_fd("exit", 1);
	if (nb_args > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (EXIT_FAILURE);
	}
	if (nb_args == 2)
	{
		close_backups(saved_in, saved_out);
		n = ft_atoi(args[1]);
		if (n >= 0 && n <= 255)
			free_and_exit(shell, n);
		else
			free_and_exit(shell, shell->prev_status);
	}
	close_backups(saved_in, saved_out);
	free_and_exit(shell, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
