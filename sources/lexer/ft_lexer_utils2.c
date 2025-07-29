/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 16:27:25 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	handle_single_quote_state(t_lexer *lexer)
{
	if (lexer->state == STATE_NORMAL)
	{
		lexer->state = STATE_SINGLE_QUOTE;
		lexer->type = WORD_SINGLE_QUOTE;
	}
	else if (lexer->state == STATE_SINGLE_QUOTE)
	{
		lexer->state = STATE_NORMAL;
		finish_token_with_type(lexer);
	}
	else if (lexer->state == STATE_DOUBLE_QUOTE)
		add_char(&(lexer->tmp_token), lexer->input[lexer->pos.x][lexer->pos.y]);
}

void	handle_double_quote_state(t_lexer *lexer)
{
	if (lexer->state == STATE_NORMAL)
		lexer->state = STATE_DOUBLE_QUOTE;
	else if (lexer->state == STATE_DOUBLE_QUOTE)
	{
		lexer->state = STATE_NORMAL;
		lexer->type = WORD_DOUBLE_QUOTE;
	}
	else if (lexer->state == STATE_SINGLE_QUOTE)
		add_char(&(lexer->tmp_token), lexer->input[lexer->pos.x][lexer->pos.y]);
}

void	handle_space_state(t_lexer *lexer)
{
	if (lexer->state == STATE_DOUBLE_QUOTE || lexer->state == STATE_SINGLE_QUOTE)
		add_char(&(lexer->tmp_token), lexer->input[lexer->pos.x][lexer->pos.y]);
}

void	handle_normal_state(t_lexer *lexer)
{
	add_char(&(lexer->tmp_token), lexer->input[lexer->pos.x][lexer->pos.y]);
}

int	check_if_not_normal_state(t_lexer *lexer)
{
	if (lexer->state != STATE_NORMAL)
	{
		if (lexer->tmp_token)
			ft_lstclear(&(lexer->tmp_token), free);
		if (lexer->tokens)
			clear_tokens_lst(&lexer->tokens);
		write(2, "error: missing quote\n", 22);
		return (1);
	}
	return (0);
}
