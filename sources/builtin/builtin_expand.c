/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/25 17:52:10 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_dollar(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

static char	*get_expanded_result(t_env **head, char *input)
{
	t_env	*current;

	if (!head || !*head || !input)
		return (NULL);
	current = *head;
	while (current)
	{
		if (!ft_strcmp(input, current->key))
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

char	*builtin_expand(char *input, t_env *env)
{
	char	*result;
	int		dollar_index;

	if (!input || env)
		return (NULL);
	dollar_index = check_dollar(input);
	if (dollar_index < 0)
		return (NULL);
	else
		result = get_expanded_result(&env, &input[dollar_index + 1]);
	if (!result)
		return (NULL);
	return (result);
}
