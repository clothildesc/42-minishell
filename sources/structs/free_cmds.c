/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:08:33 by cscache           #+#    #+#             */
/*   Updated: 2025/08/18 15:08:57 by cscache          ###   ########.fr       */
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
		if (tmp->file)
			free(tmp->file);
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
	clear_args_lst(&cmd->args);
	clear_redirs_lst(&cmd->redirs);
}
