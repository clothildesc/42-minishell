/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_lst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:22 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/10 17:38:16 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	ft_lstadd_args(t_arg **lst, t_arg *new)
{
	t_arg	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
		}
		else
		{
			*lst = new;
		}
	}
}

void	ft_lstadd_redir(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
		}
		else
		{
			*lst = new;
		}
	}
}

void	create_args_lst(t_token *token, t_cmd *cmd)
{
	t_arg	*new_arg;

	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return ;
	ft_bzero(new_arg, sizeof(t_arg));
	new_arg->arg = ft_strdup(token->value);
	if (!new_arg->arg)
	{
		free(new_arg);
		return ;
	}
	new_arg->to_exp = false;
	if (token->to_exp == true)
		new_arg->to_exp = true;
	new_arg->next = NULL;
	ft_lstadd_args(&cmd->args, new_arg);
}

void	create_redir_lst(t_token *token, t_cmd *cmd)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	ft_bzero(new_redir, sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->file = ft_strdup(token->next->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return ;
	}
	new_redir->redir = ft_strdup(token->value);
	if (!new_redir->redir)
	{
		free(new_redir);
		return ;
	}
	ft_lstadd_redir(&cmd->fds, new_redir);
}
