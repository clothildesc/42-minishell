/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/12 16:14:25 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	handle_operators(t_lexer *lexer, char c)
{
	add_char(&lexer->tmp_token, c);
	if ((c == '<' && lexer->input[lexer->pos + 1] == '<') || \
		(c == '>' && lexer->input[lexer->pos + 1] == '>'))
	{
		(lexer->pos)++;
		add_char(&lexer->tmp_token, lexer->input[lexer->pos]);
	}
	create_token(lexer, false);
}

static int	is_next_char_operator(t_lexer *lexer)
{
	char	next;

	next = lexer->input[lexer->pos + 1];
	return (next == '|' || next == '<' || next == '>');
}

static void	enter_quote_state(t_lexer *lexer, char quote_char)
{
	if (lexer->state == NORMAL && lexer->tmp_token)
	{
		if (lexer->input[lexer->pos + 1] && !ft_isspace(lexer->input[lexer->pos + 1]))
			create_token(lexer, true);
		else
			create_token(lexer, false);
	}
	if (quote_char == '\'')
	{
		lexer->state = SINGLE_QUOTE;
		lexer->to_exp = false;
	}
	else
		lexer->state = DOUBLE_QUOTE;
}

void	process_normal_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'' || c == '"')
		enter_quote_state(lexer, c);
	else if (ft_isspace(c))
	{
		// if (lexer->pos != 0 && lexer->input[lexer->pos - 1] == '\'')
		// 	create_token(lexer, true);
		// else
			create_token(lexer, false);
	}
	else if (c == '|' || c == '<' || c == '>')
		handle_operators(lexer, c);
	else
	{
		add_char(&lexer->tmp_token, c);
		if (is_next_char_operator(lexer))
			create_token(lexer, false);
	}
}
