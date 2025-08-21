/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 00:23:31 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_a_builtin(char *name)
{
	if (!name)
		return (0);
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

bool	is_parent_builtin(char *name)
{
	if (!name)
		return (false);
	if (!ft_strcmp(name, "cd"))
		return (true);
	if (!ft_strcmp(name, "exit"))
		return (true);
	if (!ft_strcmp(name, "export"))
		return (true);
	if (!ft_strcmp(name, "unset"))
		return (true);
	return (false);
}

int	dispatch_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!ft_strcmp(cmd->name, "env"))
			return (builtin_env(shell->env));
	else if (!ft_strcmp(cmd->name, "echo"))
			return (builtin_echo(cmd->args));
	else if (!ft_strcmp(cmd->name, "pwd"))
			return (builtin_pwd());
	else if (!ft_strcmp(cmd->name, "cd"))
			return (builtin_cd(cmd->args, shell->env));
	else if (!ft_strcmp(cmd->name, "unset"))
			return (builtin_unset(&shell->env, cmd->args));
	else if (!ft_strcmp(cmd->name, "export"))
			return (builtin_export(shell->env, cmd->args));
	// else if (!ft_strcmp(cmd->name, "exit"))
	//	return (builtin_exit(shell, cmd->args));
	return (EXIT_FAILURE);
}

int	exec_builtin_simple(t_cmd *cmd, t_shell *shell)
{
	int exit_code;
	int saved_in;
	int saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
			return (EXIT_FAILURE);
	if (prepare_redirections(cmd) == -1)
	{
		close(saved_in);
		close(saved_out);
		return (EXIT_FAILURE);
	}
	apply_redirections(cmd);
	exit_code = dispatch_builtin(cmd, shell);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (exit_code);
}

int	exec_builtin_in_parent(t_cmd *cmd, t_shell *shell)
{
	int exit_code = EXIT_SUCCESS;
	int saved_in;
	int saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
			return (EXIT_FAILURE);
	if (prepare_redirections(cmd) == -1)
	{
		close(saved_in);
		close(saved_out);
		return (EXIT_FAILURE);
	}
	apply_redirections(cmd);
	if (!ft_strcmp(cmd->name, "cd"))
			exit_code = builtin_cd(cmd->args, shell->env);
	else if (!ft_strcmp(cmd->name, "unset"))
			exit_code = builtin_unset(&shell->env, cmd->args);
	else if (!ft_strcmp(cmd->name, "export"))
			exit_code = builtin_export(shell->env, cmd->args);
	// else if (!ft_strcmp(cmd->name, "exit")) exit_code = builtin_exit(shell, cmd->args);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (exit_code);
}
