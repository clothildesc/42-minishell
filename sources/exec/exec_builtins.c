/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/27 10:39:48 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	execute_child_builtin(t_cmd *cmd, t_shell *shell, \
									int fd_i, int fd_o)
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
			free_and_exit(shell, EXIT_FAILURE);
		manage_dup(cmd, fd_i, fd_o);
		exit(execute_builtins(cmd, shell));
	}
	cmd->pid = pid;
}

int	exec_builtin_simple(t_cmd *cmd, t_shell *shell, int fd_i, int fd_o)
{
	int		status;
	int		exit_code;

	execute_child_builtin(cmd, shell, fd_i, fd_o);
	waitpid(cmd->pid, &status, 0);
	exit_code = get_exit_code(status);
	return (exit_code);
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
		return (EXIT_FAILURE);
	if (prepare_redirections(cmd) == -1)
	{
		close(saved_in);
		close(saved_out);
		free_and_exit(shell, EXIT_FAILURE);
	}
	manage_dup(cmd, fd_i, fd_o);
	exit_code = execute_parent_builtins(cmd, shell);
	if (dup2(saved_in, STDIN_FILENO) != STDIN_FILENO \
	|| dup2(saved_out, STDOUT_FILENO) != STDOUT_FILENO)
		exit (EXIT_FAILURE);
	close(saved_in);
	close(saved_out);
	return (exit_code);
}
