/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:32:59 by cscache           #+#    #+#             */
/*   Updated: 2025/07/30 18:29:14 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static int	is_word_type(t_token_type type)
{
	return (type == WORD || type == WORD_SINGLE_QUOTE || \
		type == WORD_DOUBLE_QUOTE);
}

static int	check_error_pipe(t_token *current)
{
	if (!current->prev || !current->next)
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	else if ((current->prev->type != WORD) && \
			(current->prev->type != WORD_DOUBLE_QUOTE) && \
			(current->prev->type != WORD_SINGLE_QUOTE))
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	else if (current->next->type == PIPE || current->prev->type == PIPE)
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	return (0);
}

static int	check_error_redir(t_token *current)
{
	if (!current->next)
		return (ft_putendl_fd(SYNTAX_ERROR_REDIR, 2), 2);
	else if (!is_word_type(current->next->type))
		return (ft_putendl_fd(SYNTAX_ERROR_REDIR, 2), 2);
	return (0);
}

int	check_syntax_errors(t_token *lst_tokens)
{
	t_token	*current;

	if (!lst_tokens)
		return (0);
	current = lst_tokens;
	while (current)
	{
		if (current->type == PIPE)
			return (check_error_pipe(current));
		if (current->type == REDIR_IN || current->type == REDIR_OUT \
		|| current->type == HERE_DOC || current->type == APPEND_OUT)
			return (check_error_redir(current));
		current = current->next;
	}
	return (0);
}
