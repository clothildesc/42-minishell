/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/26 15:59:03 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

void	execute_shell(char *input, t_shell *shell)
{
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->tokens = ft_lexer(input, shell);
	if (!shell->tokens)
	{
		g_exit_status = EXIT_SUCCESS;
		return ;
	}
	get_syntax_errors(shell->tokens);
	if (g_exit_status != EXIT_SUCCESS)
	{
		clear_tokens_lst(&shell->tokens);
		shell->prev_status = g_exit_status;
		return ;
	}
	//display_lexer_results(shell->tokens);
	shell->ast = parse_pipe(shell, &shell->tokens);
	execution(shell->ast, shell);
	clear_tokens_lst(&shell->tokens);
	clear_ast(&shell->ast);
	shell->prev_status = g_exit_status;
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
		while (true)
		{
			line = readline("minishell> ");
			if (line == NULL)
			{
				clear_env_lst(&shell.env);
				return (EXIT_FAILURE);
			}
			if (*line)
			{
				add_history(line);
				execute_shell(line, &shell);
			}
			free(line);
		}
		clear_env_lst(&shell.env);
	}
	return (shell.status);
}
