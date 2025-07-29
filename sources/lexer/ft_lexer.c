/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 13:08:59 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	init_struct_lexer(t_lexer *l)
{
	if (!l)
		return ;
	ft_bzero(l, sizeof(t_lexer));
	l->state = STATE_NORMAL;
}

t_token	*ft_lexer(char **input)
{
	t_lexer		lexer;
	t_char_type	char_type;
	int			i;

	init_struct_lexer(&lexer);
	lexer.input = input;
	while (lexer.input[i])
	{
		while (lexer.input[i][lexer.pos])
		{
			char_type = classify_char_type(lexer.input[lexer.pos]);
			if (char_type == CHAR_SINGLE_QUOTE)
				handle_single_quote_state(&lexer);
			else if (char_type == CHAR_DOUBLE_QUOTE)
				handle_double_quote_state(&lexer);
			else if (char_type == CHAR_SPACE)
				handle_space_state(&lexer);
			else
				handle_normal_state(&lexer);
			(lexer.pos)++;
		}
		if (check_if_not_normal_state(&lexer))
			return (NULL);
		finish_token_with_type(lexer);
		i++;
	}
	return (lexer.tokens);
}

#include <stdio.h>

int	main(int ac, char *av[])
{
	t_token	*tokens;
	
	if (ac < 2)
		return (1);
	tokens = ft_lexer(av);
	if (!tokens)
		return (1);
	while (tokens->next)
	{
		printf("%s\n", tokens->value);
		tokens = tokens->next;
	}
	clear_tokens_lst(tokens);
	return (0);
}
