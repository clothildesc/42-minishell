/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:48:37 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/09 18:49:13 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_delete(t_env *to_delete)
{
	if (to_delete->next != NULL)
		to_delete->next->prev = to_delete->prev;
	to_delete->prev->next = to_delete->next;
	free(to_delete->key);
	free(to_delete->value);
	free(to_delete);
}

void	ft_unset(t_env **env, char *to_delete)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strcmp(to_delete, current->key) == 0)
		{
			ft_delete(current);
			return ;
		}
		current = current->next;
	}
}
