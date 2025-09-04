/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 09:55:36 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/04 13:05:59 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	execute_shell(char *input, t_shell *shell)
{
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->tokens = ft_lexer(input, shell);
	if (!shell->tokens)
	{
		shell->status = EXIT_SUCCESS;
		return ;
	}
	get_syntax_errors(shell);
	if (shell->status != EXIT_SUCCESS)
	{
		clear_tokens_lst(&shell->tokens);
		shell->prev_status = shell->status;
		return ;
	}
	shell->ast = parse_pipe(shell, &shell->tokens);
	execution(shell->ast, shell);
	reset_exec(shell);
	return ;
}

static int	process_line(t_shell *shell, char *line)
{
	if (line == NULL && g_signal_received == 0)
	{
		write(1, "exit\n", 6);
		clear_shell(shell);
		return (0);
	}
	if (line && *line)
	{
		add_history(line);
		execute_shell(line, shell);
	}
	return (1);
}

static int	handle_backup_error(t_shell *shell, int *backup)
{
	if (*backup == -1)
	{
		clear_shell(shell);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	handle_signal_and_input(t_shell *shell, int *backup, char **line)
{
	g_signal_received = 0;
	*backup = dup(STDIN_FILENO);
	if (handle_backup_error(shell, backup))
		return (EXIT_FAILURE);
	signal(SIGINT, ft_handler_sigint);
	*line = readline("minishell> ");
	signal(SIGINT, SIG_IGN);
	if (!*line)
	{
		if (g_signal_received)
		{
			if (dup2(*backup, STDIN_FILENO) == -1)
			{
				clear_shell(shell);
				ft_close_fd(backup);
				return (EXIT_FAILURE);
			}
			return (EXIT_SUCCESS);
		}
		ft_close_fd(backup);
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}

int	main_loop(t_shell *shell)
{
	char	*line;
	int		backup;

	backup = -1;
	while (true)
	{
		if (handle_signal_and_input(shell, &backup, &line) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		ft_close_fd(backup);
		if (!process_line(shell, line))
		{
			free(line);
			clear_shell(shell);
			return (EXIT_FAILURE);
		}
		if (g_signal_received)
			shell->prev_status = g_signal_received;
		free(line);
	}
	return (EXIT_SUCCESS);
}
