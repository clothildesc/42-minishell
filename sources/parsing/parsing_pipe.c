/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:07:21 by cscache           #+#    #+#             */
/*   Updated: 2025/08/27 00:10:23 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*new_pipe;

	if (!right || !left)
		return (NULL);
	new_pipe = malloc(sizeof (t_ast));
	if (!new_pipe)
		return (clear_ast(&left), clear_ast(&right), NULL);
	new_pipe->node_type = NODE_PIPE;
	new_pipe->data.binary.left = left;
	new_pipe->data.binary.right = right;
	return (new_pipe);
}

t_ast	*parse_pipe(t_shell *shell, t_token **tokens)
{
	t_ast	*left;
	t_ast	*new_pipe;
	t_ast	*right;

	left = parse_cmd(tokens, shell);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_cmd(tokens, shell);
		if (!right)
			return (clear_ast(&left), NULL);
		new_pipe = create_pipe_node(left, right);
		if (!new_pipe)
			return (clear_ast(&left), clear_ast(&right), NULL);
		left = new_pipe;
	}
	return (left);
}
