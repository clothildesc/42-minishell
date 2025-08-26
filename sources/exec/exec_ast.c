/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:43:49 by cscache           #+#    #+#             */
/*   Updated: 2025/08/26 14:56:20 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	execute_ast(t_ast *node, t_shell *shell, int fd_i, int fd_o)
{
	int	pipefd[2];

	if (node->node_type == NODE_PIPE)
	{	
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			free_and_exit(shell, EXIT_FAILURE);
		}
		execute_ast(node->data.binary.left, shell, fd_i, pipefd[1]);
		close(pipefd[1]);
		execute_ast(node->data.binary.right, shell, pipefd[0], fd_o);
		if (pipefd[0] != -1)
			close(pipefd[0]);
		if (pipefd[1] != -1)
			close(pipefd[1]);
	}
	if (node->node_type == NODE_CMD)
		g_exit_status = execute_cmd(node, shell, fd_i, fd_o);
}

void	execution(t_ast *ast, t_shell *shell)
{
	int		fd_i;
	int		fd_o;
	t_cmd	*cmd;

	fd_i = STDIN_FILENO;
	fd_o = STDOUT_FILENO;
	if (shell->ast->node_type == NODE_CMD)
	{
		cmd = shell->ast->data.cmd.cmd;
		if (is_parent_builtin(cmd->name))
			g_exit_status = exec_builtin_in_parent(cmd, shell, fd_i, fd_o);
		else
			g_exit_status = execute_cmd(ast, shell, fd_i, fd_o);
	}
	else
		execute_ast(ast, shell, fd_i, fd_o);
}
