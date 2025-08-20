/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/20 17:35:18 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_a_builtin(char *name)
{
	if (!ft_strcmp(name, "env"))
		return (1);
	else if (!ft_strcmp(name, "pwd"))
		return (1);
	else if (!ft_strcmp(name, "cd"))
		return (1);
	else if (!ft_strcmp(name, "unset"))
		return (1);
	else if (!ft_strcmp(name, "export"))
		return (1);
	else if (!ft_strcmp(name, "echo"))
		return (1);
	else
		return (0);
}

int	exec_builtin(char *name, char **args, t_shell *shell)
{
	if (!ft_strcmp(name, "env"))
		return (builtin_env(shell->env));
	else if (!ft_strcmp(name, "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(name, "echo"))
		return (builtin_echo(args));
	else if (!ft_strcmp(name, "cd"))
		return (builtin_cd(args, shell->env));
	else if (!ft_strcmp(name, "unset"))
		return (builtin_unset(&shell->env, args));
	else if (!ft_strcmp(name, "export"))
		return (builtin_export(shell->env, args));
	else
		return (EXIT_FAILURE);
}

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
