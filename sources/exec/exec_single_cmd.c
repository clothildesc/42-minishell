/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:41:18 by cscache           #+#    #+#             */
/*   Updated: 2025/08/25 15:15:51 by cscache          ###   ########.fr       */
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

int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	else
		return (EXIT_FAILURE);
}

static void	execute_child(t_cmd *cmd, char **env_array, int fd_infile, int fd_outfile)
{
	pid_t	pid;

	printf("debut execute child cmd normale\n");
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
		printf("avant simple dup cmd normale\n");
		simple_dup(cmd, fd_infile, fd_outfile);
		printf("apres simple dup cmd normale + avant execve\n");
		execve(cmd->abs_path, cmd->args, env_array);
		printf("aprs execve\n");
		perror("execve");
		exit(EXIT_CMD_NOT_FOUND);
	}
	cmd->pid = pid;
	printf("fin execute child cmd normale\n");
}

static int	fork_and_execute(t_cmd *cmd, t_shell *shell, int fd_infile, int fd_outfile)
{
	int		status;
	char	**env_array;
	int		exit_code;

	printf("debut fork and execute\n");
	env_array = lst_env_to_array(shell->env);
	if (!env_array)
	{
		perror("malloc env_array");
		return (EXIT_FAILURE);
	}
	status = prepare_cmd(cmd, shell->env);
	printf("apres prepare cmd\n");
	if (status != EXIT_SUCCESS)
		return (status);
	printf("avant execute child\n");
	execute_child(cmd, env_array, fd_infile, fd_outfile);
	printf("apres execute child et avant waitpid cmd normale\n");
	waitpid(cmd->pid, &status, 0);
	printf("apres waitpid cmd normal\n");
	exit_code = get_exit_code(status);
	free_tab_chars(env_array);
	printf("fin fork and execute\n");
	return (exit_code);
}

int	execute_cmd(t_ast *node, t_shell *shell, int fd_infile, int fd_outfile)
{
	t_cmd	*cmd;

	if (!node)
		return (EXIT_FAILURE);
	handle_all_heredocs(node);
	cmd = node->data.cmd.cmd;
	if (!cmd->name)
		return (EXIT_SUCCESS);
	if (is_a_builtin(cmd->name))
		return (exec_builtin_simple(cmd, shell, fd_infile, fd_outfile));
	return (fork_and_execute(cmd, shell, fd_infile, fd_outfile));
}
