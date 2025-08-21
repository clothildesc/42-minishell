/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 00:22:07 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	fork_and_execute(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**env_array;

	env_array = lst_env_to_array(shell->env);
	if (!env_array)
	{
		perror("malloc env_array");
		exit(EXIT_FAILURE);
	}
	status = prepare_cmd(cmd, shell->env);
	if (status != EXIT_SUCCESS)
		return (status);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
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
	waitpid(pid, &status, 0);
	free_tab_chars(env_array);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}

int	execute_command(t_shell *shell)
{
	t_cmd	*cmd;

	if (!shell->ast)
		return (EXIT_FAILURE);
	if (handle_all_heredocs(shell->ast) == -1)
		return (EXIT_FAILURE);
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
