/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utils_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:05:25 by barmarti          #+#    #+#             */
/*   Updated: 2025/09/09 11:06:32 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	get_exp_value(t_token *token, t_shell *shell, t_arg *new_arg)
{
	char	*src;
	char	*input;

	input = ft_strdup(token->value);
	src = builtin_expand(input, shell, NULL);
	if (input)
		free(input);
	if (src)
	{
		new_arg->arg = ft_strdup(src);
		free(src);
		return ;
	}
	else
		new_arg->arg = ft_strdup(token->value);
}

void	get_token_value(t_token *token, t_arg *new_arg)
{
	char	*src;
	char	*tmp;

	src = token->value;
	if (token->state == STATE_SINGLE_QUOTE && src[0] && src[0] == '$')
	{
		tmp = ft_strjoin("'", src);
		if (!tmp)
		{
			return ;
		}
		new_arg->arg = ft_strjoin(tmp, "'");
		free(tmp);
	}
	else
		new_arg->arg = ft_strdup(src);
}
