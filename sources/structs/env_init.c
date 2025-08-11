/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:29:31 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/09 19:23:28 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
			new->prev = last;
		}
		else
		{
			*lst = new;
		}
	}
}

static void	make_env_node(char *env, t_env **head)
{
	int		i;
	t_env	*new;

	new = malloc(sizeof(t_env));
	new->key = NULL;
	new->value = NULL;
	new->prev = NULL;
	new->next = NULL;
	if (!new)
		return ;
	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
		{
			new->key = ft_strndup(env, i);
			new->value = ft_strdup(&env[i + 1]);
			break ;
		}
		i++;
	}
	ft_lstadd_back_env(head, new);
}

t_env	*get_env(char **envp)
{
	int		i;
	t_env	*env;

	env = NULL;
	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		make_env_node(envp[i], &env);
		i++;
	}
	return (env);
}
