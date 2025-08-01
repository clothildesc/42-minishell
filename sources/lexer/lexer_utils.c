/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 13:58:41 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	manage_operators(t_lexer *lexer, char c)
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

static int	is_operator(t_lexer *lexer)
{
	char	next;

	next = lexer->input[lexer->pos + 1];
	return (next == '|' || next == '>' || next == '>');
}

void	handle_normal_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
	{
		if (lexer->state == STATE_NORMAL && lexer->tmp_token)
			create_token(lexer, true);
		lexer->state = STATE_SINGLE_QUOTE;
		if (lexer->input[lexer->pos + 1] == '$')
			lexer->to_exp = false;
	}
	else if (c == '"')
	{
		if (lexer->state == STATE_NORMAL && lexer->tmp_token)
			create_token(lexer, true);
		lexer->state = STATE_DOUBLE_QUOTE;
	}
	else if (c == ' ')
		create_token(lexer, true);
	else if (c == '|' || c == '<' || c == '>')
		manage_operators(lexer, c);
	else
	{
		add_char(&lexer->tmp_token, c);
		if (is_operator(lexer))
			create_token(lexer, false);
	}
}

void	handle_single_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
		lexer->state = STATE_NORMAL;
	else
		add_char(&lexer->tmp_token, c);
}

void	handle_double_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '"')
		lexer->state = STATE_NORMAL;
	else
		add_char(&lexer->tmp_token, c);
}

int	check_if_not_normal_state(t_lexer *lexer)
{
	if (lexer->state != STATE_NORMAL)
	{
		if (lexer->tmp_token)
		{
			ft_lstclear(&(lexer->tmp_token), free);
			lexer->tmp_token = NULL;
		}
		if (lexer->tokens)
			clear_tokens_lst(&lexer->tokens);
		write(2, "error: missing quote\n", 22);
		return (1);
	}
	return (0);
}
