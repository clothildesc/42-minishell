/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/07/31 12:12:20 by cscache          ###   ########.fr       */
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
		printf("Value: %s | Type: %u\n", lst_tokens->value, lst_tokens->type);
		lst_tokens = lst_tokens->next;
	}
	clear_tokens_lst(&head);
}

int	execute_shell(char *input)
{
	t_token	*lst_tokens;
	t_shell	shell;

	init_all_structs(&shell);
	lst_tokens = ft_lexer(input, &shell);
	//display_lexer_results(lst_tokens);
	shell.exit_status = get_syntax_error_status(lst_tokens);
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
