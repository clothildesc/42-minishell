/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredocs_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 17:58:26 by cscache           #+#    #+#             */
/*   Updated: 2025/09/01 18:00:21 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	read_and_write_heredoc(int fd, char *limiter)
{
	char	*line;
	int		limiter_reached;

	limiter_reached = 0;
	remove_echoctl();
	while (true)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 \
			&& line[ft_strlen(limiter)] == '\n')
		{
			limiter_reached = 1;
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	active_echoctl();
	if (!limiter_reached && g_signal_received != 130)
		ft_putendl_fd("\nbash: warning: here-doc delimited by eof", 2);
}

void	close_prev_fd_heredoc(t_ast *node)
{
	t_cmd	*cmd;

	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
	{
		close_prev_fd_heredoc(node->data.binary.left);
		close_prev_fd_heredoc(node->data.binary.right);
	}
	else if (node->node_type == NODE_CMD)
	{
		cmd = node->data.cmd.cmd;
		ft_close_fd(cmd->fd_heredoc);
	}
}

void	execute_child_heredoc(t_ast *root, t_cmd *cmd, char *limiter, int fd_heredoc)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork heredoc");
		return ;
	}
	if (pid == 0)
	{
		close_prev_fd_heredoc(root);
		set_up_signals_child(true);
		read_and_write_heredoc(fd_heredoc, limiter);
		ft_close_fd(fd_heredoc);
		if (g_signal_received)
			exit(g_signal_received);
		else
			exit(EXIT_SUCCESS);
	}
	cmd->pid_heredoc = pid;
}
