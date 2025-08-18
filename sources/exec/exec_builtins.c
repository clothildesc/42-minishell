/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/18 15:03:55 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_a_builtin(char *name)
{
	if (ft_strcmp(name, "env") == 0)
		return (1);
	else if (ft_strcmp(name, "pwd") == 0)
		return (1);
	else if (ft_strcmp(name, "cd") == 0)
		return (1);
	else if (ft_strcmp(name, "unset") == 0)
		return (1);
	else if (ft_strcmp(name, "export") == 0)
		return (1);
	else if (ft_strcmp(name, "echo") == 0)
		return (1);
	else
		return (0);
}

static int	exec_builtins(char *name, t_arg *args, t_shell *shell)
{
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env(shell->env));
	else if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(args));
	else if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(args, shell->env));
	else if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(&shell->env, args));
	else if (ft_strcmp(name, "export") == 0)
		return (builtin_export(shell->env, args));
	else
		return (EXIT_FAILURE);
}

int	traverse_ast_and_exec_builtin(t_ast *node, t_shell *shell)
{
	char	*name;
	t_arg	*args;

	if (!node)
		return (EXIT_FAILURE);
	if (node->node_type == NODE_PIPE)
	{
		traverse_ast_and_exec_builtin(node->data.binary.left, shell);
		traverse_ast_and_exec_builtin(node->data.binary.right, shell);
	}
	else if (node->node_type == NODE_CMD)
	{
		name = node->data.cmd.cmd->name;
		args = node->data.cmd.cmd->args;
		if (is_a_builtin(name))
			return (exec_builtins(name, args, shell));
	}
	return (EXIT_FAILURE);
}
