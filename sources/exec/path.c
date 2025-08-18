/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:23:10 by cscache           #+#    #+#             */
/*   Updated: 2025/08/18 17:48:16 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*check_path(char **path_folders, char *cmd)
{
	int		i;
	char	*tmp;
	char	*path;

	i = 0;
	while (path_folders[i])
	{
		tmp = ft_strjoin(path_folders[i], "/");
		if (!tmp)
			return (NULL);
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!path)
			return (NULL);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *envp[], char *cmd)
{
	int		i;
	char	**path_folders;
	char	*path;

	i = 0;
	path_folders = NULL;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (!envp[i])
		return (NULL);
	path_folders = ft_split(envp[i] + 5, ':');
	if (!path_folders)
		return (NULL);
	path = check_path(path_folders, cmd);
	free_tab_chars(path_folders);
	return (path);
}

char	**get_args(const char *str)
{
	char	**args;

	args = ft_lexer(str);
	if (!args)
	{
		free_tab_chars(args);
		return (NULL);
	}
	return (args);
}

void	cmd_not_found(char **args, t_shell *shell)
{
	ft_putstr_fd("bash Error: command not found -> ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd("\n", 2);
	free_tab_chars(args);
	free_struct(p);
	exit_code(127);
}

void	execute_cmd(t_shell *shell, int i)
{
	char	**args;
	char	*cmd_path;
	int		to_free;

	to_free = 0;
	args = get_args(p->cmds[i]);
	if (!args)
		return (free_struct(p), exit_code(1));
	if (!args[0])
		return (free_tab_chars(args), free_struct(p), exit_code(1));
	if (ft_strchr(args[0], '/') && access(args[0], F_OK | X_OK) == 0)
		cmd_path = args[0];
	else
	{
		cmd_path = get_path(p->envp, args[0]);
		to_free = 1;
	}
	if (!cmd_path)
		cmd_not_found(args, p);
	execve(cmd_path, args, p->envp);
	perror("[Pipex] Error: execve");
	if (to_free)
		free(cmd_path);
	return (free_tab_chars(args), free_struct(p), exit_code(126));
}
