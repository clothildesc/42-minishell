/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/06/20 15:17:03 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_lexer.h"

void	handle_single_quote_state(t_lexer *l)
{
	if (l->state == STATE_NORMAL)
		l->state = STATE_SINGLE_QUOTE;
	else if (l->state == STATE_SINGLE_QUOTE)
		l->state = STATE_NORMAL;
	else if (l->state == STATE_DOUBLE_QUOTE)
		add_char(&(l->tmp_token), &(l->input[l->pos]));
}

void	handle_double_quote_state(t_lexer *l)
{
	if (l->state == STATE_NORMAL)
		l->state = STATE_DOUBLE_QUOTE;
	else if (l->state == STATE_DOUBLE_QUOTE)
		l->state = STATE_NORMAL;
	else if (l->state == STATE_SINGLE_QUOTE)
		add_char(&(l->tmp_token), &(l->input[l->pos]));
}

void	handle_space_state(t_lexer *l)
{
	if (l->state == STATE_DOUBLE_QUOTE || l->state == STATE_SINGLE_QUOTE)
		add_char(&(l->tmp_token), &(l->input[l->pos]));
	else if (l->state == STATE_NORMAL)
		finish_token(&(l->tmp_token), &(l->lst_tokens));
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
