/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/09 11:55:18 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static char	*get_var_to_expand(char *input, int space_index)
{
	char	*var_to_exp;

	var_to_exp = ft_substr(input, 0, space_index);
	if (!var_to_exp)
		return (NULL);
	return (var_to_exp);
}

static char	*find_env_var(t_env *env, char *var_to_exp, \
						char *input, int space_index)
{
	char	*result;

	while (env)
	{
		if (!ft_strcmp(var_to_exp, env->key))
		{
			result = ft_strjoin(env->value, &input[space_index]);
			return (result);
		}
		env = env->next;
	}
	return (NULL);
}

static int	get_var_end_index(char *input)
{
	int	i;

	i = 0;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	return (i);
}

static char	*get_expanded_result(t_shell *shell, char *input)
{
	char	*var_to_exp;
	char	*result;
	int		var_end_index;

	if (!shell->env || !shell || !input)
		return (NULL);
	if (!ft_strcmp(input, "?"))
		return (ft_itoa(shell->prev_status));
	var_end_index = get_var_end_index(input);
	if (var_end_index == 0)
		return (NULL);
	var_to_exp = get_var_to_expand(input, var_end_index);
	if (!var_to_exp)
		return (NULL);
	result = find_env_var(shell->env, var_to_exp, input, var_end_index);
	free(var_to_exp);
	return (result);
}

char	*builtin_expand(char *input, t_shell *shell, char *result)
{
	char	*exp;
	int		dollar_index;
	char	*new_result;

	if (!input || !shell->env)
		return (result);
	dollar_index = get_char_index(input, '$');
	if (dollar_index < 0)
		return (result);
	exp = get_expanded_result(shell, &input[dollar_index + 1]);
	if (!exp)
		return (result);
	if (!result)
		result = ft_strdup(input);
	new_result = ft_strnjoin(result, exp, dollar_index);
	free(result);
	free(exp);
	if (!new_result)
		return (NULL);
	return (builtin_expand(new_result, shell, new_result));
}
