/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:32:59 by cscache           #+#    #+#             */
/*   Updated: 2025/07/31 15:12:02 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static int	is_word_type(t_token_type type)
{
	return (type == WORD_EXPAND || type == WORD_NO_EXPAND);
}

static int	check_error_pipe(t_token *current)
{
	if (!current->prev || !current->next)
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	else if ((current->prev->type != WORD_EXPAND) && \
			(current->prev->type != WORD_NO_EXPAND))
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	else if (current->next->type == PIPE || current->prev->type == PIPE)
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	return (0);
}

static int	check_error_redir(t_token *current)
{
	if (!current->next)
		return (ft_putendl_fd(SYNTAX_ERROR_REDIR, 2), 1);
	else if (!is_word_type(current->next->type))
		return (ft_putendl_fd(SYNTAX_ERROR_REDIR, 2), 1);
	return (0);
}

static int	check_syntax_errors(t_token *lst_tokens, t_token_type type)
{
	t_token	*current;

	if (!lst_tokens)
		return (0);
	current = lst_tokens;
	while (current)
	{
		if (current->type == type)
		{
			if (type == PIPE && check_error_pipe(current))
				return (2);
			if ((type == REDIR_IN || type == REDIR_OUT || \
				type == HERE_DOC || type == APPEND_OUT) && \
				check_error_redir(current))
				return (2);
		}
		current = current->next;
	}
	return (0);
}

int	get_syntax_error_status(t_token *lst_tokens)
{
	if (check_syntax_errors(lst_tokens, PIPE))
		return (2);
	else if (check_syntax_errors(lst_tokens, REDIR_IN))
		return (2);
	else if (check_syntax_errors(lst_tokens, REDIR_OUT))
		return (2);
	else if (check_syntax_errors(lst_tokens, HERE_DOC))
		return (2);
	else if (check_syntax_errors(lst_tokens, APPEND_OUT))
		return (2);
	return (0);
}
