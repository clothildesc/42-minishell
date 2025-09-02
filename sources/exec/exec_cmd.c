/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/09/02 14:41:17 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cmd_not_found(t_cmd *cmd)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd->name, 2);
	ft_putendl_fd(": command not found", 2);
	clear_cmd(cmd);
	return (EXIT_CMD_NOT_FOUND);
}

int	get_exit_code(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		else if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		g_signal_received = 128 + sig;
		return (g_signal_received);
	}
	else
		return (EXIT_FAILURE);
}

static void	execute_child(t_cmd *cmd, char **env_array, int fd_i, int fd_o)
{
	dump_fds("debut child");
	if (prepare_redirections(cmd) == -1)
		free_child_and_exit(cmd, env_array, EXIT_FAILURE);
	manage_dup(cmd, fd_i, fd_o);
	dump_fds("execute child - apres manage dup");
	set_up_signals_child(false);
	execve(cmd->abs_path, cmd->args, env_array);
	perror("execve");
	free_child_and_exit(cmd, env_array, EXIT_CMD_NOT_FOUND);
}

static int	fork_and_execute(t_cmd *cmd, t_shell *shell, int fd_i, int fd_o)
{
	pid_t	pid;
	char	**env_array;
	int		status;

	env_array = lst_env_to_array(shell->env);
	if (!env_array)
	{
		perror("malloc env_array");
		return (EXIT_FAILURE);
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
		execute_child(cmd, env_array, fd_i, fd_o);
	shell->pids[get_index_pid()] = pid;
	free_tab_chars(env_array);
	return (EXIT_SUCCESS);
}

int	execute_cmd(t_ast *node, t_shell *shell, int fd_i, int fd_o)
{
	t_cmd	*cmd;

	if (!node)
		return (EXIT_FAILURE);
	cmd = node->data.cmd.cmd;
	if (!cmd->name)
		return (EXIT_SUCCESS);
	if (is_a_builtin(cmd->name))
		return (exec_builtin_simple(cmd, shell, fd_i, fd_o));
	return (fork_and_execute(cmd, shell, fd_i, fd_o));
}
