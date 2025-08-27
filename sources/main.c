/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/27 16:44:13 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_received = 0;

void	execute_shell(char *input, t_shell *shell)
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
	//display_lexer_results(shell->tokens);
	shell->ast = parse_pipe(shell, &shell->tokens);
	execution(shell->ast, shell);
	clear_tokens_lst(&shell->tokens);
	clear_ast(&shell->ast);
	shell->prev_status = shell->status;
	return ;
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_shell	shell;
	int		backup;

	(void)av;
	if (ac == 1)
	{
		init_all_structs(&shell, envp);
		rl_catch_signals = 0;
		set_up_signals_parent();
		while (true)
		{
			g_signal_received = 0;
			backup = dup(STDIN_FILENO);
			signal(SIGINT, ft_handler_sigint);
			line = readline("minishell> ");
			signal(SIGINT, SIG_IGN);
			if (g_signal_received)
				dup2(backup, STDIN_FILENO);
			close(backup);
			if (line == NULL && g_signal_received == 0)
			{
				ft_putstr_fd("exit\n", STDOUT_FILENO);
				clear_env_lst(&shell.env);
				return (EXIT_FAILURE);
			}
			if (line && *line)
			{
				add_history(line);
				execute_shell(line, &shell);
			}
			if (g_signal_received)
				shell.prev_status = g_signal_received;
			free(line);
		}
		clear_env_lst(&shell.env);
	}
	return (shell.status);
}
