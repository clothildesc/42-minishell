/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/21 16:25:28 by cscache          ###   ########.fr       */
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

void	execute_child_builtin(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		clear_shell(shell);
		exit_code(1);
	}
	if (pid == 0)
	{
		apply_redirections(cmd);
		execute_builtin_in_child(cmd, shell->env);
	}
	cmd->pid = pid;
}

int	exec_builtin_in_child(t_cmd *cmd, t_shell *shell)
{
	if (!ft_strcmp(cmd->name, "env"))
		return (builtin_env(shell->env));
	else if (!ft_strcmp(cmd->name, "echo"))
		return (builtin_echo(cmd->args));
	else
		return (EXIT_SUCCESS);
}

int	exec_builtin_in_parent(t_cmd *cmd, t_shell *shell)
{
	int	saved[2];
	int	exit_code;

	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	apply_redirections(cmd);
	if (!ft_strcmp(cmd->name, "pwd"))
		exit_code = builtin_pwd();
	else if (!ft_strcmp(cmd->name, "cd"))
		exit_code = builtin_cd(cmd->args, shell->env);
	else if (!ft_strcmp(cmd->name, "unset"))
		exit_code = builtin_unset(&shell->env, cmd->args);
	else if (!ft_strcmp(cmd->name, "export"))
		exit_code = builtin_export(shell->env, cmd->args);
	if (dup2(saved[0], cmd->fd_in) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	if (dup2(saved[1], cmd->fd_out) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	return (exit_code);
}

// int	exec_builtin(char *name, char **args, t_shell *shell)
// {
// 	if (!ft_strcmp(name, "env"))
// 		return (builtin_env(shell->env));
// 	else if (!ft_strcmp(name, "echo"))
// 		return (builtin_echo(args));
// 	else if (!ft_strcmp(name, "pwd"))
// 		return (builtin_pwd());
// 	else if (!ft_strcmp(name, "cd"))
// 		return (builtin_cd(args, shell->env));
// 	else if (!ft_strcmp(name, "unset"))
// 		return (builtin_unset(&shell->env, args));
// 	else if (!ft_strcmp(name, "export"))
// 		return (builtin_export(shell->env, args));
// 	else
// 		return (EXIT_SUCCESS);
// }

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
