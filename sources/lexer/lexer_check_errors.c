/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:32:59 by cscache           #+#    #+#             */
/*   Updated: 2025/08/26 12:32:06 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static int	check_error_pipe(t_token *current)
{
	if (!current->prev || !current->next)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("'|'", 2);
		return (1);
	}
	else if (current->next->type == TOKEN_PIPE)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("'|'", 2);
		return (1);
	}
	else if (current->prev->type != TOKEN_WORD)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("'|'", 2);
		return (1);
	}
	return (0);
}

static int	is_redir(t_token *current)
{
	return (current->type == TOKEN_REDIR_IN || \
			current->type == TOKEN_APPEND_OUT || \
			current->type == TOKEN_HERE_DOC || \
			current->type == TOKEN_REDIR_OUT);
}

static int	check_error_redirs(t_token *current)
{
	if (!current->next)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("'newline'", 2);
		return (1);
	}
	else if (current->next->type == TOKEN_PIPE)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("'|'", 2);
		return (1);
	}
	else if (is_redir(current->next))
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd(current->next->value, 2);
		return (1);
	}
	else if (current->next->type != TOKEN_WORD)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("'newline'", 2);
		return (1);
	}
	return (0);
}

// int	handle_special_char(t_token *token)
// {
// 	t_token *current;

// 	if (!ft_strcmp(token, "\n"))
// 		return (EXIT_CMD_NOT_FOUND);
// 	else if (!ft_strcmp(token, " "))
// 	{
		
// 	}
// }

int	get_syntax_error_status(t_token *lst_tokens)
{
	t_token	*current;

	if (!lst_tokens)
		return (0);
	current = lst_tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE && check_error_pipe(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == TOKEN_HERE_DOC && check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == TOKEN_APPEND_OUT && \
			check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == TOKEN_REDIR_IN && check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == TOKEN_REDIR_OUT && \
			check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
