/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 09:55:36 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/29 18:18:20 by barmarti         ###   ########.fr       */
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
	printf("======== APRES LEXER ============\n");
	display_lexer_results(shell->tokens);
	get_syntax_errors(shell);
	if (shell->status != EXIT_SUCCESS)
	{
		clear_tokens_lst(&shell->tokens);
		shell->prev_status = shell->status;
		return ;
	}
	printf("======== APRES CHECK SYNTAX ============\n");
	display_lexer_results(shell->tokens);
	shell->ast = parse_pipe(shell, &shell->tokens);
	printf("======== APRES AST ============\n");
	display_lexer_results(shell->tokens);
	execution(shell->ast, shell);
	printf("======== APRES EXEC ============\n");
	display_lexer_results(shell->tokens);
	clear_tokens_lst(&shell->tokens);
	printf("======== APRES CLEAR TOKENS ============\n");
	display_lexer_results(shell->tokens);
	clear_ast(&shell->ast);
	shell->prev_status = shell->status;
	return ;
}

int	process_line(t_shell *shell, char *line)
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
