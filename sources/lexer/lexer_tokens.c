/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/10 11:49:36 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	add_to_lst_tokens(t_token **lst, t_token *new)
{
	t_token	*last;

	last = NULL;
	if (*lst)
	{
		last = *lst;
		while (last->next)
			last = last->next;
		new->prev = last;
		last->next = new;
	}
	else
		*lst = new;
}

static void	reset_tmp_token(t_lexer *lexer)
{
	ft_lstclear(&lexer->tmp_token, free);
	lexer->tmp_token = NULL;
	lexer->state = STATE_NORMAL;
	lexer->to_exp = true;
	lexer->to_join = false;
}

static void	set_to_join(t_lexer *lexer)
{
	int	pos;

	pos = lexer->pos;
	while (lexer->input[pos] && (lexer->input[pos] == '"' || \
		lexer->input[pos] == '\''))
		pos++;
	if (lexer->input[pos] && !ft_isspace(lexer->input[pos]))
		lexer->to_join = true;
	else
		lexer->to_join = false;
}

static t_token	*set_new_token(t_lexer *lexer, t_token *new_token, \
	char *token_value, bool single_quote)
{
	ft_bzero(new_token, sizeof(t_token));
	new_token->value = token_value;
	new_token->to_exp = lexer->to_exp;
	if (single_quote)
		new_token->state = STATE_SINGLE_QUOTE;
	else
		new_token->state = lexer->state;
	new_token->to_join = lexer->to_join;
	new_token->type = determine_token_type(lexer);
	add_to_lst_tokens(&lexer->tokens, new_token);
	return (new_token);
}

void	create_token(t_lexer *lexer, bool to_join, bool single_quote)
{
	char	*token_value;
	t_token	*new_token;

	if (to_join)
		set_to_join(lexer);
	if (lexer->tmp_token)
	{
		token_value = create_token_value(lexer);
		if (!token_value)
		{
			ft_lstclear(&lexer->tmp_token, free);
			return ;
		}
		new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			ft_lstclear(&lexer->tmp_token, free);
			free(token_value);
			return ;
		}
		set_new_token(lexer, new_token, token_value, single_quote);
		reset_tmp_token(lexer);
	}
}
