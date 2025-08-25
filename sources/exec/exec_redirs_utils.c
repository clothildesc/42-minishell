/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 11:40:22 by cscache           #+#    #+#             */
/*   Updated: 2025/08/25 15:07:57 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	simple_dup(t_cmd *cmd, int fd_infile, int fd_outfile)
{
	printf("simple dup\n");
	printf("fd_in: %d\n", cmd->fd_in);
	printf("fd_out: %d\n", cmd->fd_out);
	printf("fd_infile: %d\n", fd_infile);
	printf("fd_outfile: %d\n", fd_outfile);
	if (cmd->fd_in != -1)
	{
		printf("fd_innnn\n");
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			perror("dup2 1");
		close(cmd->fd_in);
		if (fd_infile != STDIN_FILENO)
			close(fd_infile);
	}
	else if (fd_infile != STDIN_FILENO)
	{
		printf("coucou fd_infile\n");
		if (dup2(fd_infile, STDIN_FILENO) == -1)
			perror("dup2 2");
		close(fd_infile);
	}
	if (cmd->fd_out != -1)
	{
		printf("coucou fd_out\n");
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
		printf("coucou fd_outfile\n");
		if (dup2(fd_outfile, STDOUT_FILENO) == -1)
			perror("dup2 4");
		close(fd_outfile);
	}
	printf("fin simple dup\n");
}
