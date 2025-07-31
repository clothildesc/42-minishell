/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/31 15:13:10 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

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

static t_token_type	determine_token_type_operator(t_lexer *lexer)
{
	char	first;

	first = *(char *)lexer->tmp_token->content;
	if (first == '|')
		return (PIPE);
	if (first == '<')
	{
		if (lexer->tmp_token->next && \
			*(char *)lexer->tmp_token->next->content == '<')
			return (HERE_DOC);
		return (REDIR_IN);
	}
	if (first == '>')
	{
		if (lexer->tmp_token->next \
			&& *(char *)lexer->tmp_token->next->content == '>')
			return (APPEND_OUT);
		return (REDIR_OUT);
	}
	return (WORD_EXPAND);
}

t_token_type	determine_token_type(t_lexer *lexer)
{
	if (!lexer->tmp_token)
		return (UNDEFINED);
	if (lexer->to_exp)
		return (WORD_EXPAND);
	if (!lexer->to_exp)
		return (WORD_NO_EXPAND);
	return (determine_token_type_operator(lexer));
	return (WORD_EXPAND);
}

void	clear_tokens_lst(t_token **lst)
{
	t_token	*last;

	if (!lst)
		return ;
	while (*lst)
	{
		last = (*lst)->next;
		free((*lst)->value);
		free(*lst);
		*lst = last;
	}
	*lst = NULL;
}
