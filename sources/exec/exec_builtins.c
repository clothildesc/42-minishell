/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/09/02 18:47:48 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	execute_child_builtin(t_cmd *cmd, t_shell *shell, \
									int fd_i, int fd_o)
{
	int	status;

	if (prepare_redirections(cmd) == -1)
		free_and_exit(shell, EXIT_FAILURE);
	manage_dup(cmd, fd_i, fd_o);
	//ft_close_fd(1 - index);
	set_up_signals_child(false);
	status = execute_builtins(cmd, shell);
	free_and_exit(shell, status);
}

int	exec_builtin_simple(t_cmd *cmd, t_shell *shell, int fd_i, int fd_o)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		execute_child_builtin(cmd, shell, fd_i, fd_o);
	shell->pids[shell->pid_index++] = pid;
	return (EXIT_SUCCESS);
}

static void	close_backups(int saved_in, int saved_out)
{
	ft_close_fd(saved_in);
	ft_close_fd(saved_out);
}

int	exec_builtin_in_parent(t_cmd *cmd, t_shell *shell, int fd_i, int fd_o)
{
	int	exit_code;
	int	saved_in;
	int	saved_out;

	exit_code = EXIT_SUCCESS;
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
	{
		close_backups(saved_in, saved_out);
		return (EXIT_FAILURE);
	}
	if (prepare_redirections(cmd) == -1)
	{
		close_backups(saved_in, saved_out);
		free_and_exit(shell, EXIT_FAILURE);
	}
	manage_dup(cmd, fd_i, fd_o);
	//ft_close_fd(1 - index);
	exit_code = execute_parent_builtins(cmd, shell);
	if (dup2(saved_in, STDIN_FILENO) != STDIN_FILENO \
	|| dup2(saved_out, STDOUT_FILENO) != STDOUT_FILENO)
		free_and_exit(shell, EXIT_FAILURE);
	close_backups(saved_in, saved_out);
	return (clear_cmd(cmd), exit_code);
}
