/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:01 by cscache           #+#    #+#             */
/*   Updated: 2025/08/15 19:16:38 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	free_args(char **result, int i)
{
	if (!result)
		return ;
	while (i >= 0)
	{
		if (result[i])
			free(result[i]);
		i--;
	}
	if (result)
		free(result);
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
}

void	clear_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->name)
		free(cmd->name);
	clear_args_lst(&cmd->args);
	if (cmd->fd_infile > 2)
		close(cmd->fd_infile);
	if (cmd->fd_outfile > 2)
		close(cmd->fd_outfile);
}

void	clear_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	clear_ast(&(*ast)->left);
	clear_ast(&(*ast)->right);
	if ((*ast)->cmds)
	{
		clear_cmd((*ast)->cmds);
		free((*ast)->cmds);
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
