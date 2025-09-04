/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 09:44:34 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/04 15:27:25 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	close_all_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	while (i != shell->nb_pipes)
	{
		ft_close_fd(&shell->pipes[i][0]);
		ft_close_fd(&shell->pipes[i][1]);
		i++;
	}
}

void	close_files(t_cmd *cmd)
{
	ft_close_fd(&cmd->fd_in);
	ft_close_fd(&cmd->fd_out);
	ft_close_fd(&cmd->fd_heredoc);
}

void	close_all_command_fds(t_ast *node)
{
	t_cmd	*cmd;

	cmd = node->data.cmd.cmd;
	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
	{
		close_all_command_fds(node->data.binary.left);
		close_all_command_fds(node->data.binary.right);
	}
	else if (node->node_type == NODE_CMD)
		close_files(cmd);
}

void	close_all_fds_and_pipes(t_shell *shell)
{
	close_all_pipes(shell);
	close_all_command_fds(shell->ast);
}
