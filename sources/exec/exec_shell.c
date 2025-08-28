/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 09:55:36 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/28 14:40:06 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

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
	shell->ast = parse_pipe(shell, &shell->tokens);
	execution(shell->ast, shell);
	clear_tokens_lst(&shell->tokens);
	clear_ast(&shell->ast);
	shell->prev_status = shell->status;
	return ;
}

int	process_line(t_shell *shell, char *line)
{
	if (line == NULL && g_signal_received == 0)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		clear_env_lst(&shell->env);
		return (0);
	}
	if (line && *line)
	{
		add_history(line);
		execute_shell(line, shell);
	}
	return (1);
}
