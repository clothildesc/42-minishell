/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:01:17 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/13 17:19:43 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	free_args(char **result, int i)
{
	while (i >= 0)
	{
		if (result[i])
			free(result[i]);
		i--;
	}
	if (result[i])
		free(result[i]);
}

// bool	is_pipe(t_token *lst_token)
// {
// 	if (lst_token->type == TOKEN_PIPE)
// 		return (true);
// 	return (false);
// }

t_token	*find_pipe(t_token *lst_token)
{
	t_token	*current;

	current = lst_token;
	while (current && current->type != TOKEN_PIPE)
		current = current->next;
	if (current && current->type == TOKEN_PIPE)
		return (current);
	else
		return (NULL);
}

t_cmd	*parse_cmd_name(t_cmd *new, char *cmd_name, t_env *env)
{
	char	*cmd_expanded;

	cmd_expanded = builtin_expand(cmd_name, env);
	if (cmd_expanded)
		new->name = ft_strdup(cmd_expanded);
	else
		new->name = ft_strdup(cmd_name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	return (new);
}
