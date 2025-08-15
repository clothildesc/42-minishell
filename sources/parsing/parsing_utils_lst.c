/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_lst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:32:22 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/15 18:46:08 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	ft_lstadd_args(t_arg **lst, t_arg *new)
{
	t_arg	*last;
	char	*old_arg;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			if (last->to_join)
			{
				old_arg = last->arg;
				last->arg = ft_strjoin(last->arg, new->arg);
				free(old_arg);
				free(new->arg);
				free(new);
			}
			else
				last->next = new;
		}
		else
			*lst = new;
	}
}

void	create_args_lst(t_token *token, t_cmd *cmd, t_env *env)
{
	t_arg	*new_arg;
	char	*arg_expanded;

	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return ;
	ft_bzero(new_arg, sizeof(t_arg));
	if (token->to_exp == true)
	{
		arg_expanded = builtin_expand(token->value, env);
		if (arg_expanded)
		{
			new_arg->arg = ft_strdup(arg_expanded);
			free(arg_expanded);
		}
		else
			new_arg->arg = ft_strdup(token->value);
	}
	else
		new_arg->arg = ft_strdup(token->value);
	if (!new_arg->arg)
	{
		free(new_arg);
		return ;
	}
	new_arg->to_join = token->to_join;
	new_arg->next = NULL;
	ft_lstadd_args(&cmd->args, new_arg);
}

// void	set_redir_fd(t_token *token, t_cmd *cmd)
// {
// 	if (token->type == TOKEN_REDIR_IN)
// 		cmd->fd_infile = open_infile(token->next->value);
// 	else if (token->type == TOKEN_HERE_DOC)
// 		cmd->fd_infile = create_here_doc(token->next->value);
// 	else if (token->type == TOKEN_REDIR_OUT)
// 		cmd->fd_outfile = open_outfile(token->next->value, token->type);
// 	else if (token->type == TOKEN_APPEND_OUT)
// 		cmd->fd_outfile = open_outfile(token->next->value, token->type);
// }
