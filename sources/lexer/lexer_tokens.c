/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 22:44:37 by clothildesc      ###   ########.fr       */
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

char	*create_token_value(t_lexer *lexer)
{
	char		*token_value;
	t_list	*current;
	int			i;

	if (!(lexer->tmp_token))
		return (NULL);
	token_value = malloc(sizeof(char) * (ft_lstsize(lexer->tmp_token) + 1));
	if (!token_value)
		return (NULL);
	i = 0;
	current = lexer->tmp_token;
	while (current)
	{
		token_value[i++] = *(char *)current->content;
		current = current->next;
	}
	token_value[i] = 0;
	return (token_value);
}

void	add_to_lst_tokens(t_token **lst, t_token *new)
{
	t_token	*last;

	last = NULL;
	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
	}
	else
		*lst = new;
}
t_token_type	determine_token_type(t_lexer *lexer)
{
	char	first;

	if (!lexer->tmp_token)
		return UNDEFINED;
	if (lexer->state == STATE_SINGLE_QUOTE)
		return WORD_SINGLE_QUOTE;
	if (lexer->state == STATE_DOUBLE_QUOTE)
		return WORD_DOUBLE_QUOTE;
	first = *(char *)lexer->tmp_token->content;
	if (first == CHAR_PIPE)
		return PIPE;
	if (first == CHAR_REDIR_IN)
	{
		if (lexer->tmp_token->next &&\
			*(char *)lexer->tmp_token->next->content == CHAR_REDIR_IN)
			return HERE_DOC;
		return REDIR_IN;
	}
	if (first == CHAR_REDIR_OUT)
	{
		if (lexer->tmp_token->next \
			&& *(char *)lexer->tmp_token->next->content == CHAR_REDIR_OUT)
			return APPEND_OUT;
		return REDIR_OUT;
	}
	return WORD;
}

void	create_token(t_lexer *lexer)
{
	char					*token_value;
	t_token				*new_token;

	if (lexer->tmp_token)
	{
		token_value = create_token_value(lexer);
		if (!token_value)
			return ;
		new_token = malloc(sizeof(t_token));
		if (!new_token)
		{
			free(token_value);
			token_value = NULL;
			return ;
		}
		new_token->value = token_value;
		new_token->type = determine_token_type(lexer);
		new_token->next = NULL;
		add_to_lst_tokens(&lexer->tokens, new_token);
		ft_lstclear(&lexer->tmp_token, free);
		lexer->tmp_token = NULL;
		lexer->state = STATE_NORMAL;
	}
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
