/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/08/21 12:04:42 by cscache          ###   ########.fr       */
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
	int		i;
	t_env	*current;
	char	**all_paths;
	char	*abs_path;

	if (!cmd || !env)
		return (NULL);
	i = 0;
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

static int	execute_cmd(t_cmd *cmd, t_env *env)
{
	int	status;

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
	execve(cmd->abs_path, cmd->args, lst_env_to_array(env));
	return (EXIT_SUCCESS);
}

int	exec_one_cmd(t_shell *shell)
{
	t_cmd	*cmd;

	if (!shell->ast)
		return (EXIT_FAILURE);
	if (handle_all_heredocs(shell->ast) == 1);
		return (EXIT_FAILURE);
	cmd = shell->ast->data.cmd.cmd;
	apply_redirections(cmd);
	if (!cmd->name)
		return (EXIT_SUCCESS);
	if (is_a_builtin(cmd->name))
		return (exec_builtin(cmd->name, cmd->args, shell));
	else
		return (execute_cmd(cmd, shell->env));
}

// int	traverse_ast_and_exec_cmd(t_ast *node, t_shell *shell)
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
// 	}
// 	return (EXIT_FAILURE);
// }