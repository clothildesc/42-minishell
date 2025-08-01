/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 18:08:58 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_lexer_results(t_token *lst_tokens)
{
	t_token	*head;

	if (!lst_tokens)
		return ;
	head = lst_tokens;
	while (lst_tokens)
	{
		printf("VALUE: [%s]				TYPE: [%u]	TO_EXP: [%d]	TO_JOIN: [%d]\n", \
		lst_tokens->value, lst_tokens->type, \
		lst_tokens->to_exp, lst_tokens->to_join);
		lst_tokens = lst_tokens->next;
	}
	clear_tokens_lst(&head);
}

void	display_ast_results(t_ast *ast)
{
	int		i;
	t_cmd	*current;

	if (!ast)
		return ;
	current = ast->cmds;
	if (current)
		ft_printf("je suis dans current\n");
	while (current)
	{
		i = 0;
		ft_printf("=============================\n");
		ft_printf("CMD = [%s]\n", current->name);
		while (current->args[i])
		{
			ft_printf("ARGS[%d]= [%s]\n", i, (char *)current->args[i]);
			i++;
		}
		current = current->next;
	}
}

int	execute_shell(char *input)
{
	t_token	*lst_tokens;
	t_shell	shell;

	init_all_structs(&shell);
	lst_tokens = ft_lexer(input, &shell);
	//display_lexer_results(lst_tokens);
	shell.exit_status = get_syntax_error_status(lst_tokens);
	set_ast(&shell, lst_tokens);
	display_ast_results(&shell.ast);
	return (shell.exit_status);
}

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			return (1);
		if (*line)
		{
			add_history(line);
			execute_shell(line);
			free(line);
		}
	}
	return (0);
}
