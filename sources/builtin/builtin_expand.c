/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/05 09:15:23 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_dollar(char *input)
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

static char	*get_expanded_result(t_shell *shell, char *input)
{
	t_env	*current;

	if (!shell->env || !shell || !input)
		return (NULL);
	current = shell->env;
	while (current)
	{
		if (!ft_strcmp(input, "?"))
			return (ft_itoa(shell->prev_status));
		else if (!ft_strcmp(input, current->key))
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

char	*builtin_expand(char *input, t_shell *shell)
{
	char	*exp;
	char	*result;
	int		dollar_index;

	if (!input || !shell->env)
		return (NULL);
	dollar_index = check_dollar(input);
	if (dollar_index < 0)
		return (NULL);
	else
	{
		exp = get_expanded_result(shell, &input[dollar_index + 1]);
		result = ft_strnjoin(input, exp, dollar_index);
		free(exp);
		if (!result)
			return (NULL);
	}
	return (result);
}
