/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:01:17 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/27 00:11:44 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

t_cmd	*parse_cmd_name(t_cmd *new, char *cmd_name, t_shell *shell)
{
	char	*cmd_expanded;

	cmd_expanded = builtin_expand(cmd_name, shell);
	if (cmd_expanded)
		new->name = cmd_expanded;
	else
		new->name = ft_strdup(cmd_name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

static void	ft_lstadd_redirs(t_redir **lst, t_redir *new)
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
			*lst = new;
	}
}

void	create_redir_lst(t_token *token, t_cmd *cmd)
{
	t_redir	*new_redir;

	if (!token || !token->next || !cmd)
		return ;
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	ft_bzero(new_redir, sizeof(t_redir));
	new_redir->target = ft_strdup(token->next->value);
	if (!new_redir->target)
	{
		free(new_redir);
		return ;
	}
	new_redir->type = token->type;
	new_redir->next = NULL;
	ft_lstadd_redirs(&cmd->redirs, new_redir);
}

static void	handle_to_join(t_arg *last, t_arg *new)
{
	char	*old_arg;

	old_arg = last->arg;
	last->arg = ft_strjoin(last->arg, new->arg);
	if (!last->arg)
	{
		last->arg = old_arg;
		free(new->arg);
		free(new);
		return ;
	}
	free(old_arg);
	free(new->arg);
	free(new);
}

void	ft_lstadd_args(t_arg **args, t_arg *new)
{
	t_arg	*last;

	last = NULL;
	if (args && new)
	{
		if (*args)
		{
			last = *args;
			while (last->next)
				last = last->next;
			if (last->to_join)
				handle_to_join(last, new);
			else
				last->next = new;
		}
		else
			*args = new;
	}
}
