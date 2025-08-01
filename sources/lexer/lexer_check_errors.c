/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:32:59 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 11:48:55 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static int	check_error_pipe(t_token *current)
{
	if (!current->prev || !current->next)
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	else if (current->prev->type != TOKEN_WORD)
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	else if (current->next->type == TOKEN_PIPE || \
		current->prev->type == TOKEN_PIPE)
		return (ft_putendl_fd(SYNTAX_ERROR_PIPE, 2), 2);
	return (0);
}

static int	check_error_redir(t_token *current)
{
	if (!current->next)
		return (ft_putendl_fd(SYNTAX_ERROR_REDIR, 2), 1);
	else if (current->next->type == TOKEN_WORD)
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
			if (type == TOKEN_PIPE && check_error_pipe(current))
				return (2);
			if ((type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || \
				type == TOKEN_HERE_DOC || type == TOKEN_APPEND_OUT) && \
				check_error_redir(current))
				return (2);
		}
		current = current->next;
	}
	return (0);
}

int	get_syntax_error_status(t_token *lst_tokens)
{
	if (check_syntax_errors(lst_tokens, TOKEN_PIPE))
		return (2);
	else if (check_syntax_errors(lst_tokens, TOKEN_REDIR_IN))
		return (2);
	else if (check_syntax_errors(lst_tokens, TOKEN_REDIR_OUT))
		return (2);
	else if (check_syntax_errors(lst_tokens, TOKEN_HERE_DOC))
		return (2);
	else if (check_syntax_errors(lst_tokens, TOKEN_APPEND_OUT))
		return (2);
	return (0);
}
