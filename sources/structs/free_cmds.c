/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:08:33 by cscache           #+#    #+#             */
/*   Updated: 2025/09/01 15:36:22 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	free_tab_chars(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	tab = NULL;
}

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
	*lst = NULL;
}

static void	close_files(t_cmd *cmd)
{
	ft_close_fd(cmd->fd_in);
	ft_close_fd(cmd->fd_out);
	ft_close_fd(cmd->fd_heredoc);
}

void	clear_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->name)
	{
		free(cmd->name);
		cmd->name = NULL;
	}
	if (cmd->abs_path)
	{
		free(cmd->abs_path);
		cmd->abs_path = NULL;
	}
	if (cmd->args)
	{
		free_tab_chars(cmd->args);
		cmd->args = NULL;
	}
	if (cmd->redirs)
	{
		clear_redirs_lst(&cmd->redirs);
		cmd->redirs = NULL;
	}
	close_files(cmd);
}
