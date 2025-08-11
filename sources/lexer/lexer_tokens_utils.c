/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 10:58:27 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 14:08:28 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	add_char(t_list **tmp_token, char c)
{
	t_list	*new;
	char	*new_char;

	new_char = malloc(sizeof(char));
	if (!new_char)
		return ;
	*new_char = c;
	new = ft_lstnew(new_char);
	if (!new)
	{
		free(new_char);
		new_char = NULL;
		return ;
	}
	ft_lstadd_back(tmp_token, new);
}

t_token_type	determine_token_type(t_lexer *lexer)
{
	char	first;

	if (!lexer->tmp_token)
		return (0);
	first = *(char *)lexer->tmp_token->content;
	if (first == '|')
		return (TOKEN_PIPE);
	if (first == '<')
	{
		if (lexer->tmp_token->next && \
			*(char *)lexer->tmp_token->next->content == '<')
			return (TOKEN_HERE_DOC);
		return (TOKEN_REDIR_IN);
	}
	if (first == '>')
	{
		if (lexer->tmp_token->next \
			&& *(char *)lexer->tmp_token->next->content == '>')
			return (TOKEN_APPEND_OUT);
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD);
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
