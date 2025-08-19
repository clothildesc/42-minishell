/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/19 15:10:37 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_a_builtin(char *name)
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

static int	exec_builtin(char *name, char **args, t_shell *shell)
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

int	traverse_ast_and_exec_builtin(t_ast *node, t_shell *shell)
{
	char	*name;
	char	**args;

	if (!node)
		return (EXIT_FAILURE);
	if (node->node_type == NODE_PIPE)
	{
		traverse_ast_and_exec_builtin(node->data.binary.left, shell);
		return (traverse_ast_and_exec_builtin(node->data.binary.right, shell));
	}
	else if (node->node_type == NODE_CMD)
	{
		name = node->data.cmd.cmd->name;
		args = node->data.cmd.cmd->args;
		if (is_a_builtin(name))
			return (exec_builtin(name, args, shell));
	}
	return (EXIT_FAILURE);
}

// static int	is_valid_executable_path(char *path)
// {
// 	struct stat	s;

// 	if (stat(path, &s) == -1)
// 	{
// 		ft_putendl_fd("bash: No such file or directory", 2);
// 		return (EXIT_FAILURE);
// 	}
// 	if (S_ISDIR(s.st_mode))
// 	{
// 		ft_putendl_fd("bash: Is a directory", 2);
// 		return (EXIT_FAILURE);
// 	}
// 	if (access(path, X_OK) != 0)
// 	{
// 		ft_putendl_fd("	bash: Permission denied", 2);
// 		return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

// int	execute_cmd(t_cmd *cmd)
// {
// 	if (!cmd)
// 		return (EXIT_FAILURE);
// 	if (ft_strchr(cmd->name, '/') && (is_valid_executable_path(cmd->name)))
// 		execve(cmd->name, cmd->args)
// 	}
// }

// int	exec_one_cmd(t_shell *shell)
// {
// 	t_cmd	*cmd;

// 	if (!shell->ast)
// 		return (EXIT_FAILURE);
// 	cmd = shell->ast->data.cmd.cmd;
// 	if (is_a_builtin(cmd->name))
// 		return (exec_builtin(cmd->name, cmd->args, shell));
// 	else
// 		return (execute_cmd(cmd));
// }
