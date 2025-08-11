/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:39:16 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/11 12:00:16 by barmarti         ###   ########.fr       */
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
	return (env[i - 1] - inpt[i - 1]);
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

int	get_value_len(char *src)
{
	int	i;

	i = 0;
	if (!src)
		return (0);
	while (src[i] && !ft_isspace(src[i]))
		i++;
	return (i);
}

int	get_key_len(char *input)
{
	int	index;

	index = 0;
	while (input[index] && input[index] != '=')
		index++;
	return (index);
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
	return (ft_strndup(input, i));
}
