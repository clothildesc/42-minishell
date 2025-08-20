/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:08:33 by cscache           #+#    #+#             */
/*   Updated: 2025/08/20 17:26:54 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	clear_args_lst(t_arg **lst)
{
	t_arg	*tmp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		if (tmp->arg)
			free(tmp->arg);
		free(tmp);
	}
	*lst = NULL;
}

void	clear_redirs_lst(t_redir **lst)
{
	t_redir	*tmp;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		if (tmp->target)
			free(tmp->target);
		free(tmp);
	}
}

void	clear_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->name)
		free(cmd->name);
	if (cmd->abs_path)
		free(cmd->abs_path);
	free_tab_chars(cmd->args);
	clear_redirs_lst(&cmd->redirs);
	if (cmd->fd_in != -1)
		close(cmd->fd_in);
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
	if (cmd->fd_heredoc != -1)
		close(cmd->fd_heredoc);
}
