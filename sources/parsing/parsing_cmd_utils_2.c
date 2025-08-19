/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:01:17 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/19 17:16:03 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	create_args_lst(t_arg **args, t_token *token, t_env *env)
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
	ft_lstadd_args(args, new_arg);
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

static char	**fill_args_array(char **args_array, t_arg *args)
{
	int		i;
	t_arg	*current;

	i = 0;
	current = args;
	while (current)
	{
		args_array[i] = ft_strdup(current->arg);
		if (!args_array[i])
		{
			clear_args_array(args_array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	args_array[i] = NULL;
	return (args_array);
}

void	lst_args_to_array(t_cmd *cmd, t_arg **args)
{
	char	**args_array;

	args_array = (char **)malloc(sizeof(char *) * (args_lst_size(*args) + 1));
	if (!args_array)
	{
		clear_args_lst(args);
		return ;
	}
	cmd->args = fill_args_array(args_array, *args);
	if (!cmd->args)
	{
		clear_args_lst(args);
		free(args_array);
		return ;
	}
	clear_args_lst(args);
}
