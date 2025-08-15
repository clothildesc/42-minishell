/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:07:21 by cscache           #+#    #+#             */
/*   Updated: 2025/08/15 19:14:02 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static t_ast	*create_cmd_node(void)
{
	t_ast	*new_cmd;

	new_cmd = malloc(sizeof(t_ast));
	if (!new_cmd)
		return (NULL);
	ft_bzero(new_cmd, sizeof(t_ast));
	new_cmd->node_type = NODE_CMD;
	new_cmd->data.cmd.cmd = malloc(sizeof(t_cmd));
	if (!new_cmd->data.cmd.cmd)
	{
		free(new_cmd);
		return (NULL);
	}
	ft_bzero(new_cmd->data.cmd.cmd, sizeof(t_cmd));
	return (new_cmd);
}

void	add_redir_to_lst(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
		}
		else
		{
			*lst = new;
		}
	}
}

void	create_redir_lst(t_token *token, t_cmd *cmd)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	ft_bzero(new_redir, sizeof(t_redir));
	new_redir->file = ft_strdup(token->next->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return ;
	}
	new_redir->type = token->type;
	new_redir->next = NULL;
	add_redir_to_lst(&cmd->redirs, new_redir);
}

static t_ast	*parse_cmd(t_token **tokens, t_env *env, bool first)
{
	t_token	*current;
	t_ast	*new_cmd;

	current = *tokens;
	new_cmd = init_cmd_node();
	if (!new_cmd)
		return (NULL);
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			if (first)
			{
				parse_cmd_name(new_cmd->data.cmd.cmd, current->value, env);
				first = false;
			}
			else
				create_args_lst(current, new_cmd->data.cmd.cmd, env);
			current = current->next;
			continue ;
		}
		if (current->type == TOKEN_REDIR_IN || \
			current->type == TOKEN_REDIR_OUT || \
			current->type == TOKEN_HERE_DOC || \
			current->type == TOKEN_APPEND_OUT)
		{
			create_redir_lst(current, new_cmd->data.cmd.cmd);
			current = current->next->next;
			continue ;
		}
		break ;
	}
	*tokens = current;
	return (new_cmd);
}

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*new_pipe;

	new_pipe = malloc(sizeof (t_ast));
	if (!new_pipe)
		return (clear_ast(&left), clear_ast(&right), NULL);
	new_pipe->node_type = NODE_PIPE;
	new_pipe->data.binary.left = left;
	new_pipe->data.binary.right = right;
	return (new_pipe);
}

t_ast	*parse_pipeline(t_shell *shell, t_token **tokens)
{
	t_ast	*left;
	t_ast	*new_pipe;
	t_ast	*right;

	left = parse_cmd(tokens, shell->env, true);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_cmd(tokens, shell->env, true);
		if (!right)
			return (clear_ast(&left), NULL);
		new_pipe = parse_pipe(left, right);
		if (!new_pipe)
			return (clear_ast(&left), clear_ast(&right), NULL);
		left = new_pipe;
	}
	return (left);
}
