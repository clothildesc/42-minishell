/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/08/20 11:47:00 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_executable_path(char *path)
{
	struct stat	s;

	if (stat(path, &s) == -1)
	{
		ft_putendl_fd("bash: No such file or directory", 2);
		return (EXIT_FAILURE);
	}
	if (S_ISDIR(s.st_mode))
	{
		ft_putendl_fd("bash: Is a directory", 2);
		return (EXIT_FAILURE);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putendl_fd("bash: Permission denied", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	execute_cmd(t_cmd *cmd, t_env *env)
{
	if (!cmd)
		return (EXIT_FAILURE);
	if (ft_strchr(cmd->name, '/') && (is_valid_executable_path(cmd->name)))
		execve(cmd->name, cmd->args, lst_env_to_array(env));
	return (EXIT_SUCCESS);
}

int	exec_one_cmd(t_shell *shell)
{
	t_cmd	*cmd;

	if (!shell->ast)
		return (EXIT_FAILURE);
	cmd = shell->ast->data.cmd.cmd;
	if (is_a_builtin(cmd->name))
		return (exec_builtin(cmd->name, cmd->args, shell));
	else
		return (execute_cmd(cmd, shell->env));
}
