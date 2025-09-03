/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/09/03 12:06:44 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

int	open_infile(char *infile)
{
	int	fd;

	if (access(infile, F_OK) == -1)
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

	if (type == APPEND_OUT)
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

void	ft_close_fd(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

static int	handle_input_redir(t_cmd *cmd, t_redir *current)
{
	ft_close_fd(&cmd->fd_in);
	if (current->type == HERE_DOC)
	{
		cmd->fd_in = dup(cmd->fd_heredoc);
		ft_close_fd(&cmd->fd_heredoc);
	}
	else
		cmd->fd_in = open_infile(current->target);
	if (cmd->fd_in == -1)
		return (-1);
	return (0);
}

int	prepare_redirections(t_cmd *cmd)
{
	t_redir	*current;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == REDIR_IN || current->type == HERE_DOC)
		{
			if (handle_input_redir(cmd, current) == -1)
				return (-1);
		}
		else if (current->type == REDIR_OUT || current->type == APPEND_OUT)
		{
			ft_close_fd(&cmd->fd_out);
			cmd->fd_out = open_outfile(current->target, current->type);
			if (cmd->fd_out == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
