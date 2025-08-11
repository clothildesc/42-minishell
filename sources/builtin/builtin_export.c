/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:09:25 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/11 16:39:15 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*create_env_node(t_env *new, char *input)
{
	new->key = get_input_key(input);
	new->value = get_input_value(input);
	if (!new->value || !new->key)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

void	update_env_value(char *input, t_env **node)
{
	free((*node)->value);
	(*node)->value = get_input_value(input);
	if (!(*node)->value)
		return ;
}

/*
- La verification d'existence fais un peu cochon je trouve
- input nulle pas fe
*/
void	builtin_export(t_env *env, char *input)
{
	t_env	*new;
	t_env	*tmp;
	char	*key;

	key = get_input_key(input);
	if (key)
	{
		tmp = env;
		while (tmp)
		{
			if (!ft_strcmp(key, tmp->key))
			{
				update_env_value(input, &tmp);
				return ;
			}
			tmp = tmp->next;
		}
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	ft_bzero(new, sizeof(t_env));
	new = create_env_node(new, input);
	if (!new)
		return ;
	ft_lstadd_back_env(&env, new);
}
