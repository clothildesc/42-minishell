/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/28 21:07:13 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_received = 0;

int	create_backup(t_shell *shell, int *backup)
{
	*backup = dup(STDIN_FILENO);
	if (*backup == -1)
	{
		clear_shell(shell);
		return (0);
	}
	return (1);
}

int	restore_backup(t_shell *shell, int backup, char *line)
{
	if (line)
		free(line);
	if (dup2(backup, STDIN_FILENO) == -1)
	{
		clear_shell(shell);
		if (backup != -1)
			close(backup);
		return (0);
	}
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*line;
	int		backup;

	(void)av;
	backup = -1;
	if (ac == 1)
	{
		init_shell(&shell, envp);
		while (true)
		{
			g_signal_received = 0;
			if (!create_backup(&shell, &backup))
				return (EXIT_FAILURE);
			signal(SIGINT, ft_handler_sigint);
			line = readline("minishell> ");
			signal(SIGINT, SIG_IGN);
			if (g_signal_received)
			{
				if (!restore_backup(&shell, backup, line))
					return (EXIT_FAILURE);
			}
			if (backup != -1)
				close(backup);
			if (!process_line(&shell, line))
			{
				free(line);
				clear_shell(&shell);
				return (EXIT_FAILURE);
			}
			if (g_signal_received)
				shell.prev_status = g_signal_received;
			free(line);
		}
		clear_shell(&shell);
	}
	return (shell.status);
}
