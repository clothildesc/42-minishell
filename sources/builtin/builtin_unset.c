/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:48:37 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/19 17:48:05 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	delete_node_env(t_env **head, t_env *to_delete)
{
	if (!to_delete)
		return ;
	if (!to_delete->prev)
		*head = to_delete->next;
	else
		to_delete->prev->next = to_delete->next;
	if (to_delete->next != NULL)
		to_delete->next->prev = to_delete->prev;
	free(to_delete->key);
	free(to_delete->value);
	free(to_delete);
}

int	builtin_unset(t_env **env, char **args)
{
	t_env	*current;
	int		i;

	if (!env || !*env)
		return (EXIT_FAILURE);
	if (!args)
		return (EXIT_SUCCESS);
	i = 0;
	while (args[i])
	{
		current = *env;
		while (current)
		{
			if (!ft_strcmp(args[i], current->key))
			{
				delete_node_env(env, current);
				break ;
			}
			current = current->next;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
