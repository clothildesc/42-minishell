/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 09:55:36 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/01 17:04:56 by cscache          ###   ########.fr       */
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
	// printf("======== APRES LEXER ============\n");
	// display_lexer_results(shell->tokens);
	get_syntax_errors(shell);
	if (shell->status != EXIT_SUCCESS)
	{
		clear_tokens_lst(&shell->tokens);
		shell->prev_status = shell->status;
		return ;
	}
	// printf("======== APRES CHECK SYNTAX ============\n");
	// display_lexer_results(shell->tokens);
	shell->ast = parse_pipe(shell, &shell->tokens);
	// printf("======== APRES AST ============\n");
	// display_lexer_results(shell->tokens);
	execution(shell->ast, shell);
	clear_ast(&shell->ast);
	clear_tokens_lst(&shell->tokens);
	clear_lexer_tmp(&shell->lexer);
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
	// printf("======== APRES CLEAR TOKENS ============\n");
	// display_lexer_results(shell->tokens);
	shell->prev_status = shell->status;
	return ;
}

static int	process_line(t_shell *shell, char *line)
{
	if (line == NULL && g_signal_received == 0)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
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
	if (!*line)
		return (ft_close_fd(*backup), EOF_RECEIVED);
	signal(SIGINT, SIG_IGN);
	if (g_signal_received)
	{
		if (line)
		{
			free(line);
			line = NULL;
		}
		if (dup2(*backup, STDIN_FILENO) == -1)
		{
			ft_close_fd(*backup);
			clear_shell(shell);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	main_loop(t_shell *shell)
{
	char	*line;
	int		backup;
	int		status;

	backup = -1;
	while (true)
	{
		status = handle_signal_and_input(shell, &backup, &line);
		if (status == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (status == EOF_RECEIVED)
			break ;
		if (backup != -1)
			close(backup);
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
