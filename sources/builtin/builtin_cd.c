/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:38:45 by cscache           #+#    #+#             */
/*   Updated: 2025/08/19 12:32:08 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//verifier que ca fonctionne aussi sans args ->renvoie vers home

// static int	args_lst_size(t_arg *args)
// {
// 	int	size;

// 	size = 0;
// 	while (args)
// 	{
// 		size++;
// 		args = args->next;
// 	}
// 	return (size);
// }

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

int	builtin_cd(t_arg *args, t_env *env)
{
	char	*path;

	if (args_lst_size(args) > 1)
	{
		ft_putendl_fd(ERROR_CD_MANY_ARGS, 2);
		return (EXIT_FAILURE);
	}
	if (!args)
		path = get_env_value(env, "HOME");
	else
		path = args->arg;
	if (chdir(path) == -1)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
