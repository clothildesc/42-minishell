/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_keep_?.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:36:00 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 11:36:01 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// int	traverse_ast_and_exec_builtin(t_ast *node, t_shell *shell)
// {
// 	char	*name;
// 	char	**args;

// 	if (!node)
// 		return (EXIT_FAILURE);
// 	if (node->node_type == NODE_PIPE)
// 	{
// 		traverse_ast_and_exec_builtin(node->data.binary.left, shell);
// 		return (traverse_ast_and_exec_builtin(node->data.binary.right, shell));
// 	}
// 	else if (node->node_type == NODE_CMD)
// 	{
// 		name = node->data.cmd.cmd->name;
// 		args = node->data.cmd.cmd->args;
// 		if (is_a_builtin(name))
// 			return (exec_builtin(name, args, shell));
// 	}
// 	return (EXIT_FAILURE);
// }
