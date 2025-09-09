/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:39:16 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/09 10:22:47 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

int	compare_key(char *env, char *input)
{
	int	i;

	i = 0;
	while (env[i] && input[i] && env[i] != '=' && input[i] != '='\
		&& env[i] == input[i])
		i++;
	return (env[i] - input[i]);
}

t_env	*get_node(t_env **head, char *key)
{
	t_env	*current;

	if (!head || !*head || !key)
		return (NULL);
	current = *head;
	while (current)
	{
		if (!ft_strcmp(key, current->key))
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	value_to_append(char *input)
{
	int	i;

	i = 1;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '=' && input[i - 1] == '+')
			return (1);
		i++;
	}
	return (0);
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
	if (!input)
		return (NULL);
	while (input[i] && input[i] != '=')
		i++;
	if (input[i] == '\0')
		return (NULL);
	if (value_to_append(input))
		return (ft_strndup(input, i - 1));
	else
		return (ft_strndup(input, i));
}
