/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/07/30 10:53:53 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_lexer_results(char *line)
{
	t_token	*tokens;
	t_token	*head;

	tokens = ft_lexer(line);
	if (!tokens)
		return ;
	head = tokens;
	while (tokens)
	{
		printf("Value: %s | Type: %u\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
	clear_tokens_lst(&head);
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
			display_lexer_results(line);
			free(line);
		}
	}
	return (0);
}
