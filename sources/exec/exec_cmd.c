/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 14:52:22 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cmd_not_found(t_cmd *cmd)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd->name, 2);
	ft_putendl_fd(": command not found", 2);
	return (EXIT_CMD_NOT_FOUND);
}

static int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (EXIT_FAILURE);
}

static void	execute_child(t_cmd *cmd, char **env_array)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		if (prepare_redirections(cmd) == -1)
			exit(EXIT_FAILURE);
		apply_redirections(cmd);
		execve(cmd->abs_path, cmd->args, env_array);
		perror("execve");
		exit(EXIT_CMD_NOT_FOUND);
	}
	cmd->pid = pid;
}

static int	fork_and_execute(t_cmd *cmd, t_shell *shell)
{
	int		status;
	char	**env_array;
	int		exit_code;

	env_array = lst_env_to_array(shell->env);
	if (!env_array)
	{
		perror("malloc env_array");
		return (EXIT_FAILURE);
	}
	status = prepare_cmd(cmd, shell->env);
	if (status != EXIT_SUCCESS)
		return (status);
	execute_child(cmd, env_array);
	waitpid(cmd->pid, &status, 0);
	exit_code = get_exit_code(status);
	free_tab_chars(env_array);
	return (exit_code);
}

int	execute_command(t_shell *shell)
{
	t_cmd	*cmd;

	if (!shell->ast)
		return (EXIT_FAILURE);
	handle_all_heredocs(shell->ast);
	cmd = shell->ast->data.cmd.cmd;
	if (!cmd->name)
		return (EXIT_SUCCESS);
	if (is_a_builtin(cmd->name))
	{
		if (is_parent_builtin(cmd->name))
			return (exec_builtin_in_parent(cmd, shell));
		return (exec_builtin_simple(cmd, shell));
	}
	return (fork_and_execute(cmd, shell));
}
