/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:38:45 by cscache           #+#    #+#             */
/*   Updated: 2025/09/03 17:51:10 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*update_oldpwd(char *key)
{
	t_env	*to_update;

	
}

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
	t_env	*old_path;
	int		i;

	old_path = update_oldpwd("OLDPWD");
	if (args && !args[1])
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
		path = args[1];
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
