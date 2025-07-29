/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 16:28:54 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	init_struct_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	ft_bzero(lexer, sizeof(t_lexer));
	lexer->state = STATE_NORMAL;
	lexer->type = UNDEFINED;
}

t_token	*ft_lexer(char **input)
{
	t_lexer		lexer;
	t_char_type	char_type;
	int			i;

	i = 0;
	init_struct_lexer(&lexer);
	lexer.input = input;
	while (lexer.input[lexer.pos.x])
	{
		lexer.pos.y = 0;
		while (lexer.input[lexer.pos.x][lexer.pos.y])
		{
			char_type = classify_char_type(lexer.input[lexer.pos.x][lexer.pos.y]);
			if (char_type == CHAR_SINGLE_QUOTE)
				handle_single_quote_state(&lexer);
			else if (char_type == CHAR_DOUBLE_QUOTE)
				handle_double_quote_state(&lexer);
			else if (char_type == CHAR_SPACE)
				handle_space_state(&lexer);
			else
				handle_normal_state(&lexer);
			(lexer.pos.y)++;
		}
		if (check_if_not_normal_state(&lexer))
			return (NULL); //implementer fonction qui attend une autre quote
		finish_token_with_type(&lexer);
		(lexer.pos.x)++;
	}
	return (lexer.tokens);
}
