/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/25 15:10:57 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static void	execute_child_builtin(t_cmd *cmd, t_shell *shell, int fd_infile, int fd_outfile)
{
	pid_t	pid;

	printf("debut exec child builtin\n");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		printf("dans enfant builtin\n");
		if (prepare_redirections(cmd) == -1)
			exit(EXIT_FAILURE);
		printf("avant simple dup\n");
		simple_dup(cmd, fd_infile, fd_outfile);
		printf("apres simple dup\n");
		exit(execute_builtins(cmd, shell));
		printf("apres execute builtin\n");
	}
	cmd->pid = pid;
	printf("fin exec child builtin\n");
}

int	exec_builtin_simple(t_cmd *cmd, t_shell *shell, int fd_infile, int fd_outfile)
{
	int		status;
	int		exit_code;

	printf("debut exec builtin simple\n");
	printf("avant exec child builtin\n");
	execute_child_builtin(cmd, shell, fd_infile, fd_outfile);
	printf("apres exec child builtin\n");
	printf("avant waitpid\n");
	waitpid(cmd->pid, &status, 0);
	printf("apres waitpid\n");
	exit_code = get_exit_code(status);
	printf("fin exec builtin simple\n");
	return (exit_code);
}

int	exec_builtin_in_parent(t_cmd *cmd, t_shell *shell, int fd_infile, int fd_outfile)
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
		return (EXIT_FAILURE);
	}
	simple_dup(cmd, fd_infile, fd_outfile);
	exit_code = execute_parent_builtins(cmd, shell);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (exit_code);
}
