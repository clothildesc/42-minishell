/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/28 17:59:54 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	handle_single_quote_state(t_lexer *l)
{
	if (l->state == STATE_NORMAL)
		l->state = STATE_SINGLE_QUOTE;
	else if (l->state == STATE_SINGLE_QUOTE)
	{
		l->state = STATE_NORMAL;
		finish_token(l, WORD_SINGLE_QUOTE);
	}
	else if (l->state == STATE_DOUBLE_QUOTE)
		add_char(&(l->tmp_token), &(l->input[l->pos]));
}

void	handle_double_quote_state(t_lexer *l)
{
	if (l->state == STATE_NORMAL)
		l->state = STATE_DOUBLE_QUOTE;
	else if (l->state == STATE_DOUBLE_QUOTE)
	{
		l->state = STATE_NORMAL;
		finish_token(l, WORD_DOUBLE_QUOTE);
	}
	else if (l->state == STATE_SINGLE_QUOTE)
		add_char(&(l->tmp_token), &(l->input[l->pos]));
}

void	handle_space_state(t_lexer *l)
{
	if (l->state == STATE_DOUBLE_QUOTE || l->state == STATE_SINGLE_QUOTE)
		add_char(&(l->tmp_token), &(l->input[l->pos]));
	else if (l->state == STATE_NORMAL)
		finish_token(l, WORD);
}

void	handle_normal_state(t_lexer *l)
{
	add_char(&(l->tmp_token), &(l->input[l->pos]));
}

int	check_if_not_normal_state(t_lexer *l)
{
	if (l->state != STATE_NORMAL)
	{
		if (l->tmp_token)
			ft_lstclear(&(l->tmp_token), free);
		if (l->lst_tokens)
			ft_lstclear(&(l->lst_tokens), free);
		write(2, "error: missing quote\n", 22);
		return (1);
	}
	return (0);
}
