/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/06/20 16:51:43 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_lexer.h"

t_char_type	classify_char_type(char c)
{
	if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '\'')
		return (CHAR_SINGLE_QUOTE);
	else if (c == '"')
		return (CHAR_DOUBLE_QUOTE);
	return (CHAR_NORMAL);
}

void	add_char(t_list **tmp_token, const char *c)
{
	t_list	*new;
	char	*new_char;

	new_char = malloc(sizeof(char));
	if (!new_char)
		return ;
	*new_char = *c;
	new = ft_lstnew(new_char);
	if (!new)
	{
		free(new_char);
		return ;
	}
	ft_lstadd_back(tmp_token, new);
}

char	*create_token(t_list *tmp_token)
{
	char	*token;
	int		i;

	if (!tmp_token)
		return (NULL);
	token = malloc(sizeof(char) * (ft_lstsize(tmp_token) + 1));
	if (!token)
		return (NULL);
	i = 0;
	while (tmp_token)
	{
		token[i] = *(char *)tmp_token->content;
		i++;
		tmp_token = tmp_token->next;
	}
	token[i] = 0;
	return (token);
}

void	finish_token(t_list **tmp_token, t_list **lst_tokens)
{
	char	*token;

	if (*tmp_token)
	{
		token = create_token(*tmp_token);
		if (token)
			ft_lstadd_back(lst_tokens, ft_lstnew(token));
		ft_lstclear(tmp_token, free);
		*tmp_token = NULL;
	}
}

char	**lst_to_array(t_list *lst_tokens)
{
	char	**tokens;
	int		i;
	t_list	*current;
	t_list	*to_free;

	tokens = malloc(sizeof(char *) * (ft_lstsize(lst_tokens) + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	current = lst_tokens;
	while (current)
	{
		tokens[i] = (char *)current->content;
		current = current->next;
		i++;
	}
	tokens[i] = NULL;
	while (lst_tokens)
	{
		to_free = lst_tokens;
		lst_tokens = lst_tokens->next;
		free(to_free);
	}
	return (tokens);
}
