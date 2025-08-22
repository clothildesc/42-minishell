/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:40:22 by cscache           #+#    #+#             */
/*   Updated: 2025/08/20 14:37:12 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	env_lst_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

static char	**fill_env_array(char **env_array, t_env *env)
{
	int		i;
	t_env	*current;
	char	*tmp;

	i = 0;
	current = env;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		if (!tmp)
			return (NULL);
		env_array[i] = ft_strjoin(tmp, current->value);
		if (!env_array[i])
		{
			free(tmp);
			free_tab_chars(env_array);
			return (NULL);
		}
		free(tmp);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	**lst_env_to_array(t_env *env)
{
	char	**env_array;

	env_array = (char **)malloc(sizeof(char *) * (env_lst_size(env) + 1));
	if (!env_array)
		return (NULL);
	fill_env_array(env_array, env);
	if (!env_array)
	{
		free(env_array);
		return (NULL);
	}
	return (env_array);
}
