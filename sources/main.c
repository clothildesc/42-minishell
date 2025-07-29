/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 21:44:05 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;
	t_token *head;

	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
			return (1);
		if (*line)
		{
			add_history(line);
			tokens = ft_lexer(line);
			if (!tokens)
				return (1);
			head = tokens;
			while (tokens)
			{
				printf("%s\n", tokens->value);
				tokens = tokens->next;
			}
			clear_tokens_lst(&head);
			free(line);
		}
	}
	return (0);
}
