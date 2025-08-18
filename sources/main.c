/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/18 17:46:57 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status = 0;

int	execute_shell(char *input, t_shell *shell)
{
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->tokens = ft_lexer(input, shell);
	shell->exit_status = get_syntax_error_status(shell->tokens);
	if (shell->exit_status != EXIT_SUCCESS)
	{
		clear_tokens_lst(&shell->tokens);
		return (shell->exit_status);
	}
	//display_lexer_results(shell->tokens);
	shell->ast = parse_pipeline(shell, &shell->tokens);
	if (shell->ast->node_type == NODE_CMD)
		shell->exit_status = exec_one_cmd(shell);
	shell->exit_status = traverse_ast_and_exec_builtin(shell->ast, shell);
	clear_tokens_lst(&shell->tokens);
	clear_ast(&shell->ast);
	return (shell->exit_status);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_shell	shell;

	(void)av;
	if (ac == 1)
	{
		init_all_structs(&shell, envp);
		while (1)
		{
			line = readline("minishell> ");
			if (line == NULL)
			{
				clear_env_lst(&shell.env);
				return (1);
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
	return (shell.exit_status);
}
