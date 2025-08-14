/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:48:37 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/14 13:38:37 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	delete_node_env(t_env *to_delete)
{
	if (!to_delete)
		return ;
	if (to_delete->next != NULL)
		to_delete->next->prev = to_delete->prev;
	to_delete->prev->next = to_delete->next;
	free(to_delete->key);
	free(to_delete->value);
	free(to_delete);
}

int	builtin_unset(t_env **env, char *to_delete)
{
	t_env	*current;

	if (!env || !*env || !to_delete)
		return (EXIT_FAILURE);
	current = *env;
	while (current)
	{
		if (!ft_strcmp(to_delete, current->key))
		{
			delete_node_env(current);
			return (EXIT_SUCCESS);
		}
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
