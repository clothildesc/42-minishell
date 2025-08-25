/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:40:22 by cscache           #+#    #+#             */
/*   Updated: 2025/08/25 16:09:30 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	simple_dup(t_cmd *cmd, int fd_infile, int fd_outfile)
{
	if (cmd->fd_in != -1)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			perror("dup2 1");
		close(cmd->fd_in);
		if (fd_infile != STDIN_FILENO)
			close(fd_infile);
	}
	else if (fd_infile != STDIN_FILENO)
	{
		if (dup2(fd_infile, STDIN_FILENO) == -1)
			perror("dup2 2");
		close(fd_infile);
	}
	if (cmd->fd_out != -1)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 3");
		}
		close(cmd->fd_out);
		if (fd_outfile != STDOUT_FILENO)
			close(fd_outfile);

	}
	else if (fd_outfile != STDOUT_FILENO)
	{
		if (dup2(fd_outfile, STDOUT_FILENO) == -1)
			perror("dup2 4");
		close(fd_outfile);
	}
}
