/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:01:17 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/19 12:39:56 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

t_cmd	*parse_cmd_name(t_cmd *new, char *cmd_name, t_env *env)
{
	char	*cmd_expanded;

	cmd_expanded = builtin_expand(cmd_name, env);
	if (cmd_expanded)
		new->name = ft_strdup(cmd_expanded);
	else
		new->name = ft_strdup(cmd_name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

static void	ft_lstadd_args(t_arg **lst, t_arg *new)
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

t_arg	*create_args_lst(t_token *token, t_cmd *cmd, t_env *env)
{
	t_arg	*new_arg;
	char	*exp;
	char	*src;

	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return ;
	ft_bzero(new_arg, sizeof(t_arg));
	exp = NULL;
	if (token->to_exp == true)
		exp = builtin_expand(token->value, env);
	if (exp)
		src = exp;
	else
		src = token->value;
	new_arg->arg = ft_strdup(src);
	if (exp)
		free(exp);
	if (!new_arg->arg)
	{
		free(new_arg);
		return ;
	}
	new_arg->to_join = token->to_join;
	ft_lstadd_args(&cmd->args, new_arg);
}

static int	args_lst_size(t_arg *args)
{
	int	size;

	size = 0;
	while (args)
	{
		size++;
		args = args->next;
	}
	return (size);
}

void	clean_args_array(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	lst_args_to_array(t_cmd *cmd, t_arg *args)
{
	char	**args_array;
	int		i;

	i = 0;
	args_array = (char **)malloc(sizeof(char *) * (args_lst_size(args) + 1));
	while (!args_array)
	{
		clear_args_lst(&args);
		return ;
	}
	while (args)
	{
		args_array[i] = ft_strdup(args);
		if (!args_array[i])
		{
			clean_args_array(args_array);
			clear_args_lst(&args);
			return ;
		}
		i++;
		args = args->next;
	}
	clean_args_lst(&args);
	args_array[i] = NULL;
	cmd->args = args_array;
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

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	ft_bzero(new_redir, sizeof(t_redir));
	new_redir->file = ft_strdup(token->next->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return ;
	}
	new_redir->type = token->type;
	new_redir->next = NULL;
	ft_lstadd_redirs(&cmd->redirs, new_redir);
}
