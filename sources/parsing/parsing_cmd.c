/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 20:03:10 by cscache           #+#    #+#             */
/*   Updated: 2025/08/15 20:35:32 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static t_ast	*init_cmd_node(void)
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

static int	is_redir(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || \
			type == TOKEN_HERE_DOC || type == TOKEN_APPEND_OUT);
}

static void	process_word(t_token **current, t_cmd *new_cmd, t_env *env)
{
	if (!new_cmd->name)
		parse_cmd_name(new_cmd->name, (*current)->value, env);
	else
		create_args_lst(current, new_cmd, env);
	*current = (*current)->next;
}

static void	process_redir(t_token **current, t_cmd *new_cmd)
{
	create_redir_lst(&current, new_cmd);
	*current = (*current)->next->next;
}

t_ast	*parse_cmd(t_token **tokens, t_env *env, bool first)
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
			process_word(&current, new_cmd->data.cmd.cmd, env);
			continue ;
		}
		if (is_redir(current->type))
		{
			process_redir(&current, new_cmd->data.cmd.cmd);
			continue ;
		}
		break ;
	}
	*tokens = current;
	return (new_cmd);
}
