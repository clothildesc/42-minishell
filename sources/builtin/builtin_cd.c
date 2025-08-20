/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:38:45 by cscache           #+#    #+#             */
/*   Updated: 2025/08/20 16:13:03 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	builtin_cd(char **args, t_env *env)
{
	char	*path;
	int		i;

	if (!args || !args[0])
		path = get_env_value(env, "HOME");
	else
	{
		i = 1;
		while (args[i])
			i++;
		if (i > 2)
		{
			ft_putendl_fd(ERROR_CD_MANY_ARGS, 2);
			return (EXIT_FAILURE);
		}
		path = args[0];
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
