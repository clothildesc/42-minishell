/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_check_errors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:32:59 by cscache           #+#    #+#             */
/*   Updated: 2025/08/26 15:02:54 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	get_syntax_errors(t_token *lst_tokens)
{
	int	exit_code;

	exit_code = handle_special_char(lst_tokens);
	if (exit_code != -1)
		g_exit_status = exit_code;
	else
		g_exit_status = get_syntax_error_status(lst_tokens);
}
