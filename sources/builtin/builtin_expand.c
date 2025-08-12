/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/12 12:15:30 by cscache          ###   ########.fr       */
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

static bool	check_uppercase(char *input, int i)
{
	if (input[i])
	{
		i++;
		while (input[i])
		{
			if (!ft_isupper(input[i]))
				return (false);
			i++;
		}
	}
	return (true);
}

static char	*get_expanded_result(t_env **head, char *input)
{
	t_env	*current;
	char	*result;

	current = *head;
	while (current)
	{
		if (!ft_strcmp(input, current->key))
		{
			result = ft_strdup(current->value);
			if (!result)
				return (NULL);
		}
		current = current->next;
	}
	return (result);
}

char	*builtin_expand(char *input, t_env *env)
{
	char	*ret;
	int		dollar_index;

	dollar_index = check_dollar(input);
	if (dollar_index < 0)
		return (NULL);
	if (!check_uppercase(input, dollar_index))
		return (NULL);
	ret = get_expanded_result(&env, &input[dollar_index + 1]);
	if (!ret)
		return (NULL);
	return (ret);
}
