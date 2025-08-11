/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/11 17:01:49 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	check_arg(char *input)
{
	int	i;

	i = 1;
	while (input[i])
	{
		if (input[0] != '$')
			return (false);
		if (!ft_isupper(input[i]))
			return (false);
		i++;
	}
	return (true);
}

static char	*get_result(t_env **head, char *input)
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

	if (!check_arg(input))
		return (NULL);
	ret = get_result(&env, &input[1]);
	if (!ret)
		return (NULL);
	return (ret);
}
