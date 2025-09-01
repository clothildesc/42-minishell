/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:40:22 by cscache           #+#    #+#             */
/*   Updated: 2025/09/01 17:28:38 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	process_dup_infile(t_cmd *cmd, int fd_i)
{
	if (cmd->fd_in != -1)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			perror("dup2 1");
		close(cmd->fd_in);
		cmd->fd_in = -1;
		if (fd_i != STDIN_FILENO)
			ft_close_fd(fd_i);
	}
	else if (fd_i != STDIN_FILENO)
	{
		if (dup2(fd_i, STDIN_FILENO) == -1)
			perror("dup2 2");
		ft_close_fd(fd_i);
	}
}

static void	process_dup_outfile(t_cmd *cmd, int fd_o)
{
	if (cmd->fd_out != -1)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			perror("dup2 3");
		close(cmd->fd_out);
		cmd->fd_out = -1;
		if (fd_o != STDOUT_FILENO)
			ft_close_fd(fd_o);
	}
	else if (fd_o != STDOUT_FILENO)
	{
		if (dup2(fd_o, STDOUT_FILENO) == -1)
			perror("dup2 4");
		ft_close_fd(fd_o);
	}
}

void	manage_dup(t_cmd *cmd, int fd_i, int fd_o)
{
	process_dup_infile(cmd, fd_i);
	process_dup_outfile(cmd, fd_o);
}
