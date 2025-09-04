/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/09/04 15:35:58 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_received = 0;

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	int		status;

	if (ac <= 2)
	{
		init_shell(&shell, envp);
		if (ac == 2)
		{
			status = test_function(&shell, av[1]);
			clear_shell(&shell);
		}
		else
		{
			status = main_loop(&shell);
			clear_shell(&shell);
			rl_clear_history();
		}
		if (status == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (shell.status);
}
