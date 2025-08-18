/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:01 by cscache           #+#    #+#             */
/*   Updated: 2025/08/18 10:45:45 by cscache          ###   ########.fr       */
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
	clear_args_lst(&cmd->args);
	clear_redirs_lst(&cmd->redirs);
}

void	clear_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	clear_ast(&(*ast)->data.binary.left);
	clear_ast(&(*ast)->data.binary.right);
	if ((*ast)->data.cmd.cmd)
	{
		clear_cmd((*ast)->data.cmd.cmd);
		free((*ast)->data.cmd.cmd);
	}
	free(*ast);
	*ast = NULL;
}

void	clear_env_lst(t_env **env)
{
	t_env	*tmp;

	if (!env || !*env)
		return ;
	while (*env)
	{	
		tmp = *env;
		*env = (*env)->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
