/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:09:25 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/11 14:41:34 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static t_env	*check_if_exist(t_env **env, char *input)
// {
// 	t_env	*current;

// 	current = *env;
// 	while (current)
// 	{
// 		if (!compare_key(current->key, input))
// 			return (current);
// 		current = current->next;
// 	}
// 	return (NULL);
// }

// static char	*get_value_content(char *src)
// {
// 	int		i;
// 	int		len;
// 	char	*dup;

// 	i = 0;
// 	len = get_value_len(src);
// 	dup = malloc(len + 1);
// 	if (!dup)
// 		return (NULL);
// 	while (src[i] && i != len)
// 	{
// 		dup[i] = src[i];
// 		i++;
// 	}
// 	dup[i] = 0;
// 	return (dup);
// }

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
void	ft_export(t_env *env, char *input)
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
