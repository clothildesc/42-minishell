/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:09:25 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/13 14:30:13 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*create_env_node(t_env *new, char *input)
{
	new->key = get_input_key(input);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	new->value = get_input_value(input);
	if (!new->value)
	{
		free(new->key);
		free(new);
		return (NULL);
	}
	return (new);
}

static void	update_env_value(char *input, t_env **node)
{
	char	*new_value;
	char	*old_value;

	new_value = get_input_value(input);
	if (value_to_append(input))
	{
		if (!new_value)
			return ;
		old_value = (*node)->value;
		(*node)->value = ft_strjoin((*node)->value, new_value);
		free(old_value);
		free(new_value);
	}
	else
	{
		free((*node)->value);
		(*node)->value = get_input_value(input);
	}
	if (!(*node)->value)
		return ;
}

static t_env	*find_or_create_env(t_env *env, char *input, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->key))
		{
			update_env_value(input, &tmp);
			return (NULL);
		}
		tmp = tmp->next;
	}
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (NULL);
	ft_bzero(tmp, sizeof(t_env));
	return (create_env_node(tmp, input));
}

static int	key_name_is_valid(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalpha(key[0]) && key[0] != '_')
		{
			ft_putendl_fd(SYNTAX_ERROR_KEY_ENV, 2);
			return (0);
		}
		else if (!ft_isalpha(key[i]) && !ft_isdigit(key[i]) && key[i] != '_')
		{
			ft_putendl_fd(SYNTAX_ERROR_KEY_ENV, 2);
			return (0);
		}
		else
			i++;
	}
	return (1);
}

void	builtin_export(t_env *env, char *input)
{
	char	*key;
	t_env	*new;

	key = get_input_key(input);
	if (!key)
		return ;
	if (!key_name_is_valid(key))
	{
		free(key);
		return ;
	}
	new = find_or_create_env(env, input, key);
	if (new)
		ft_lstadd_back_env(&env, new);
	free(key);
}
