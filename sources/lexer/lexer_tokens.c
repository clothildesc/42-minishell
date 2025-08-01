/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 15:00:26 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static char	*create_token_value(t_lexer *lexer)
{
	char		*token_value;
	t_list		*current;
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

static void	add_to_lst_tokens(t_token **lst, t_token *new)
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
	lexer->to_join = 0;
}

static void	set_to_join(t_lexer *lexer)
{
	if (lexer->input[lexer->pos] && lexer->input[lexer->pos] != ' ' )
		lexer->to_join = 1;
}

void	create_token(t_lexer *lexer, bool to_join)
{
	char	*token_value;
	t_token	*new_token;

	if (to_join)
		set_to_join(lexer);
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
		new_token->to_exp = lexer->to_exp;
		new_token->to_join = lexer->to_join;
		new_token->type = determine_token_type(lexer);
		new_token->prev = NULL;
		new_token->next = NULL;
		add_to_lst_tokens(&lexer->tokens, new_token);
		reset_tmp_token(lexer);
	}
}
