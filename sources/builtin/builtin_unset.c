/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:48:37 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/18 14:28:38 by cscache          ###   ########.fr       */
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

int	builtin_unset(t_env **env, t_arg *args)
{
	t_env	*current;

	if (!env || !*env || !args)
		return (EXIT_FAILURE);
	while (args)
	{
		current = *env;
		while (current)
		{
			if (!ft_strcmp(args->arg, current->key))
			{
				delete_node_env(current);
				break ;
			}
			current = current->next;
		}
		args = args->next;
	}
	return (EXIT_SUCCESS);
}
