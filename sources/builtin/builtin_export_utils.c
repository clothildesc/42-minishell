/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:39:16 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/13 12:28:44 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	compare_key(char *env, char *inpt)
{
	int	i;

	i = 0;
	while (env[i] && inpt[i] && env[i] != '=' && inpt[i] != '='\
		&& env[i] == inpt[i])
		i++;
	return (env[i] - inpt[i]);
}

t_env	*get_node(t_env **head, char *key)
{
	t_env	*current;

	current = *head;
	while (current)
	{
		if (ft_strcmp(key, current->key))
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*get_input_value(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '=')
			return (ft_strdup(&input[i + 1]));
		i++;
	}
	return (NULL);
}

char	*get_input_key(char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '=')
		i++;
	if (input[i] == '\0')
		return (NULL);
	if (input[i] == '=' && input[i - 1] == '+')
		return (ft_strndup(input, i - 1));
	else
		return (ft_strndup(input, i));
}
