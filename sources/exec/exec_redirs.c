/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 00:19:48 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

int	open_infile(char *infile)
{
	int	fd;

	if (access(infile, F_OK | R_OK) == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(infile, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	else
	{
		fd = open(infile, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(infile, 2);
			ft_putendl_fd(": Permission denied", 2);
		}
	}
	return (fd);
}

int	open_outfile(char *outfile, t_token_type type)
{
	int	fd;

	if (type == TOKEN_APPEND_OUT)
		fd = open(outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
	else
	fd = open(outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(outfile, 2);
		ft_putendl_fd(": Permission denied", 2);
	}
	return (fd);
}

static void	check_file_open_and_close(int fd)
{
	if (fd != -1)
		close(fd);
}

int	prepare_redirections(t_cmd *cmd)
{
	t_redir	*current;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == TOKEN_HERE_DOC)
		{
			check_file_open_and_close(cmd->fd_in);
			cmd->fd_in = cmd->fd_heredoc;
		}
		else if (current->type == TOKEN_REDIR_IN)
		{
			check_file_open_and_close(cmd->fd_in);
			cmd->fd_in = open_infile(current->target);
			if (cmd->fd_in == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIR_OUT \
				|| current->type == TOKEN_APPEND_OUT)
		{
			check_file_open_and_close(cmd->fd_out);
			cmd->fd_out = open_outfile(current->target, current->type);
			if (cmd->fd_out == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

void	apply_redirections(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
			if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
					perror("dup2");
			close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
			if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
					perror("dup2");
			close(cmd->fd_out);
	}
}
