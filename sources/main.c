/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/27 02:05:51 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_recieved = 0;

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

	(void)av;
	if (ac == 1)
	{
		init_all_structs(&shell, envp);
		set_up_signals();
		rl_catch_signals = 0;
		while (true)
		{
			g_signal_recieved = 0;
			int backup = dup(0);//check failur
			signal(2, ft_handler);
			line = readline("minishell> ");
			signal(2, SIG_IGN);
			if (g_signal_recieved) 
				dup2(backup, 0);
			close(backup);
			if (line == NULL && g_signal_recieved == 0)
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
			if (g_signal_recieved)
				shell.prev_status = g_signal_recieved;
			free(line);
		}
		clear_env_lst(&shell.env);
	}
	return (shell.status);
}
