/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 17:06:36 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{
	char	*line;
	t_token	*tokens;

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
			while (tokens)
			{
				printf("%s\n", tokens->value);
				tokens = tokens->next;
			}
			clear_tokens_lst(&tokens);
			free(line);
		}
	}
	return (0);
}
