/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_errors_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:32:59 by cscache           #+#    #+#             */
/*   Updated: 2025/09/10 10:24:27 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static int	check_error_pipe(t_token *current)
{
	if (!current->prev || !current->next)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("`|'", 2);
		return (1);
	}
	else if (current->next->type == PIPE)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("`|'", 2);
		return (1);
	}
	else if (current->prev->type != WORD)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("`|'", 2);
		return (1);
	}
	return (0);
}

static int	is_redir(t_token *current)
{
	return (current->type == REDIR_IN || \
			current->type == APPEND_OUT || \
			current->type == HERE_DOC || \
			current->type == REDIR_OUT);
}

static int	check_error_redirs(t_token *current)
{
	if (!current->next)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("`newline'", 2);
		return (1);
	}
	else if (current->next->type == PIPE)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("`|'", 2);
		return (1);
	}
	else if (is_redir(current->next))
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd(current->next->value, 2);
		return (1);
	}
	else if (current->next->type != WORD)
	{
		ft_putstr_fd(SYNTAX_ERROR_TOKEN, 2);
		ft_putendl_fd("`newline'", 2);
		return (1);
	}
	return (0);
}

int	handle_special_char(t_token *head)
{
	if (!head)
		return (-1);
	else if (!head->next && !ft_strcmp(head->value, "!"))
		return (EXIT_FAILURE);
	else if (!head->next && !ft_strcmp(head->value, "."))
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		return (EXIT_SYNTAX_ERROR);
	}
	return (-1);
}

int	get_syntax_error_status(t_token *lst_tokens)
{
	t_token	*current;

	if (!lst_tokens)
		return (EXIT_SUCCESS);
	current = lst_tokens;
	while (current)
	{
		if (current->type == PIPE && check_error_pipe(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == HERE_DOC && check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == APPEND_OUT && \
			check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == REDIR_IN && check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		else if (current->type == REDIR_OUT && \
			check_error_redirs(current))
			return (EXIT_SYNTAX_ERROR);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
