/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/27 00:29:55 by barmarti         ###   ########.fr       */
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

/*
VERIF SI MODIF POUR EXPAND EXIT STATUS PAS TOUT CASSE
*/
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
	char	*result;
	int		dollar_index;

	if (!input || !shell->env)
		return (NULL);
	dollar_index = check_dollar(input);
	if (dollar_index < 0)
		return (NULL);
	else
		result = get_expanded_result(shell, &input[dollar_index + 1]);
	if (!result)
		return (NULL);
	return (result);
}
