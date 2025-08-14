/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:07:21 by cscache           #+#    #+#             */
/*   Updated: 2025/08/13 18:09:45 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*new;

	new = malloc(sizeof (t_ast));
	if (!new)
		return (NULL);
	new->node_type = NODE_PIPE;
	new->cmds = NULL;
	new->left = left;
	new->right = right;
	return (new);
}

static t_ast	*init_ast_node(void)
{
	t_ast	*new;

	new = malloc(sizeof(t_ast));
	if (!new)
		return (NULL);
	ft_bzero(new, sizeof(t_ast));
	new->node_type = NODE_CMD;
	new->cmds = malloc(sizeof(t_cmd));
	if (!new->cmds)
	{
		free(new);
		return (NULL);
	}
	ft_bzero(new->cmds, sizeof(t_cmd));
	new->cmds->fd_infile = -1;
	new->cmds->fd_outfile = -1;
	return (new);
}

static t_ast	*create_ast_node(t_token **tokens, t_env *env, bool first)
{
	t_token	*current;
	t_ast	*new;

	new = init_ast_node();
	current = *tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (first && current && current->type == TOKEN_WORD)
		{
			parse_cmd_name(new->cmds, current->value, env);
			first = false;
		}
		else if (current && current->type != TOKEN_WORD)
		{
			set_redir_fd(current, new->cmds);
			current = current->next->next;
			continue ;
		}
		else
			create_args_lst(current, new->cmds, env);
		current = current->next;
	}
	*tokens = current;
	return (new);
}

t_ast	*set_ast(t_shell *shell, t_token **tokens)
{
	t_ast	*left;
	t_ast	*new_pipe;
	t_ast	*right;

	left = create_ast_node(tokens, shell->env, true);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = create_ast_node(tokens, shell->env, true);
		new_pipe = create_pipe_node(left, right);
		left = new_pipe;
	}
	return (left);
}
