/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:17:06 by clothildesc       #+#    #+#             */
/*   Updated: 2025/08/22 00:24:37 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_executable_path(char *path)
{
	struct stat	s;

	if (stat(path, &s) == -1)
		return (0);
	if (S_ISDIR(s.st_mode))
		return (0);
	if (access(path, X_OK) != 0)
		return (0);
	return (1);
}

static int	validate_exec_path(char *path)
{
	struct stat	s;

	if (stat(path, &s) == -1)
	{
		ft_putstr_fd("bash: ", 2);
		perror(path);
		return (EXIT_CMD_NOT_FOUND);
	}
	if (S_ISDIR(s.st_mode))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Is a directory", 2);
		return (EXIT_PERMISSION_DENIED);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("bash: ", 2);
		perror(path);
		return (EXIT_PERMISSION_DENIED);
	}
	return (EXIT_SUCCESS);
}

static char	*build_cmd_abs_path(char **all_paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*abs_path;

	i = 0;
	while (all_paths[i])
	{
		tmp = ft_strjoin(all_paths[i], "/");
		if (!tmp)
			return (NULL);
		abs_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!abs_path)
			return (NULL);
		if (is_valid_executable_path(abs_path))
			return (abs_path);
		free(abs_path);
		i++;
	}
	return (NULL);
}

static char	*get_cmd_abs_path(t_cmd *cmd, t_env *env)
{
	t_env	*current;
	char	**all_paths;
	char	*abs_path;

	if (!cmd || !env)
		return (NULL);
	current = env;
	while (current)
	{
		if (!ft_strcmp(current->key, "PATH"))
			break ;
		current = current->next;
	}
	if (!current)
		return (NULL);
	all_paths = ft_split(current->value, ':');
	if (!all_paths)
		return (NULL);
	abs_path = build_cmd_abs_path(all_paths, cmd->name);
	free_tab_chars(all_paths);
	return (abs_path);
}

int	prepare_cmd(t_cmd *cmd, t_env *env)
{
	int		status;

	if (!cmd || !env)
		return (EXIT_FAILURE);
	if (ft_strchr(cmd->name, '/'))
	{
		status = validate_exec_path(cmd->name);
		if (status != EXIT_SUCCESS)
			return (status);
		cmd->abs_path = cmd->name;
	}
	else
	{
		cmd->abs_path = get_cmd_abs_path(cmd, env);
		if (!cmd->abs_path)
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(cmd->name, 2);
			ft_putendl_fd(": command not found", 2);
			return (EXIT_CMD_NOT_FOUND);
		}
	}
	return (EXIT_SUCCESS);
}
