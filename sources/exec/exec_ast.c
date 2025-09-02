/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:43:49 by cscache           #+#    #+#             */
/*   Updated: 2025/09/02 14:55:16 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	dump_fds(const char *where)
{
	int	fd;

	fd = 3;
	while (fd < 256)
	{
		if (fcntl(fd, F_GETFD) != -1)
			ft_printf("[%s] open fd=%d\n", where, fd);
		fd++;
	}
	ft_printf("[%s] -> fin de dump\n", where);
}
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
		//dump_fds("avant ast left");
		execute_ast(node->data.binary.left, shell, fd_i, pipefd[1]);
		//dump_fds("apres ast left");
		ft_close_fd(pipefd[1]);
		//dump_fds("avant ast right");
		execute_ast(node->data.binary.right, shell, pipefd[0], fd_o);
		//dump_fds("apres ast right");
		ft_close_fd(pipefd[0]);
		//dump_fds("fin ast");
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
		perror("bash: pids malloc");
		free_and_exit(shell, EXIT_FAILURE);
	}
}

void	get_status_code(t_shell *shell)
{
	int		i;
	int		status;
	pid_t	current_pid;

	i = 0;
	while (i < shell->nb_cmds)
	{
		current_pid = waitpid(shell->pids[i], &status, 0);
		if (current_pid == shell->pids[shell->nb_cmds - 1])
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
}
