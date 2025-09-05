/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:43:49 by cscache           #+#    #+#             */
/*   Updated: 2025/09/05 10:14:00 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	execute_ast(t_ast *node, t_shell *shell, int fd_i, int fd_o)
{
	int		pipefd[2];

	if (node->node_type == NODE_PIPE)
	{	
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			free_and_exit(shell, EXIT_FAILURE);
		}
		shell->pipes[shell->nb_pipes][0] = pipefd[0];
		shell->pipes[shell->nb_pipes][1] = pipefd[1];
		shell->nb_pipes++;
		execute_ast(node->data.binary.left, shell, fd_i, pipefd[1]);
		ft_close_fd(&pipefd[1]);
		execute_ast(node->data.binary.right, shell, pipefd[0], fd_o);
		ft_close_fd(&pipefd[0]);
	}
	else if (node->node_type == NODE_CMD)
		shell->status = execute_cmd(node, shell, fd_i, fd_o);
}

void	count_cmd_nodes(t_ast *node, t_shell *shell)
{
	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
	{
		count_cmd_nodes(node->data.binary.left, shell);
		count_cmd_nodes(node->data.binary.right, shell);
	}
	else if (node->node_type == NODE_CMD)
		shell->nb_cmds++;
}

void	init_pids(t_shell *shell)
{
	shell->pids = ft_calloc(shell->nb_cmds, sizeof(pid_t));
	if (!shell->pids)
	{
		perror("minishell: pids malloc");
		free_and_exit(shell, EXIT_FAILURE);
	}
}

void	get_status_code(t_shell *shell)
{
	int		i;
	int		status;
	pid_t	current_pid;

	i = 0;
	while (i < shell->pid_index)
	{
		current_pid = waitpid(shell->pids[i], &status, 0);
		if (current_pid == shell->pids[shell->pid_index - 1])
			shell->status = get_exit_code(status);
		i++;
	}
}

void	execution(t_ast *ast, t_shell *shell)
{
	int		fd_i;
	int		fd_o;
	t_cmd	*cmd;

	if (!ast)
		return ;
	fd_i = STDIN_FILENO;
	fd_o = STDOUT_FILENO;
	handle_all_heredocs(ast, shell);
	if (shell->status == 130)
		return ;
	count_cmd_nodes(ast, shell);
	init_pids(shell);
	if (ast->node_type == NODE_CMD)
	{
		cmd = ast->data.cmd.cmd;
		if (is_parent_builtin(cmd->name))
			shell->status = exec_builtin_in_parent(cmd, shell, fd_i, fd_o);
		else
			shell->status = execute_cmd(ast, shell, fd_i, fd_o);
	}
	else
		execute_ast(ast, shell, fd_i, fd_o);
	get_status_code(shell);
	close_all_fds_and_pipes(shell);
}
