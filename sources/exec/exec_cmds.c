/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:43:49 by cscache           #+#    #+#             */
/*   Updated: 2025/08/25 16:02:13 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	execute_ast(t_ast *node, t_shell *shell, int fd_infile, int fd_outfile)
{
	int	pipefd[2];

	if (node->node_type == NODE_PIPE)
	{	
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			free_and_exit(shell, 1);
		}
		execute_ast(node->data.binary.left, shell, fd_infile, pipefd[1]);
		close(pipefd[1]);
		execute_ast(node->data.binary.right, shell, pipefd[0], fd_outfile);
		if (pipefd[0] != -1)
			close(pipefd[0]);
		if (pipefd[1] != -1)
			close(pipefd[1]);
	}
	if (node->node_type == NODE_CMD)
	{
		shell->exit_status = execute_cmd(node, shell, fd_infile, fd_outfile);
	}
}

void	execution(t_ast *ast, t_shell *shell)
{
	int		fd_infile;
	int		fd_outfile;
	t_cmd	*cmd;

	fd_infile = STDIN_FILENO;
	fd_outfile = STDOUT_FILENO;
	if (shell->ast->node_type == NODE_CMD)
	{
		cmd = shell->ast->data.cmd.cmd;
		if (is_parent_builtin(cmd->name))
			shell->exit_status = exec_builtin_in_parent(cmd, shell, fd_infile, fd_outfile);
		else
			shell->exit_status = execute_cmd(ast, shell, fd_infile, fd_outfile);
	}
	else
		execute_ast(ast, shell, fd_infile, fd_outfile);
}

