/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 22:42:47 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	init_struct_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	ft_bzero(lexer, sizeof(t_lexer));
	lexer->state = STATE_NORMAL;
}

t_char_type	classify_char_type(char c)
{
	if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '\'')
		return (CHAR_SINGLE_QUOTE);
	else if (c == '"')
		return (CHAR_DOUBLE_QUOTE);
	else if (c == '|')
		return (CHAR_PIPE);
	else if (c == '<')
		return (CHAR_REDIR_IN);
	else if (c == '>')
		return (CHAR_REDIR_OUT);
	return (CHAR_NORMAL);
}

int	check_if_not_normal_state(t_lexer *lexer)
{
	if (lexer->state != STATE_NORMAL)
	{
		if (lexer->tmp_token)
		{
			ft_lstclear(&(lexer->tmp_token), free);
			lexer->tmp_token = NULL;
		}
		if (lexer->tokens)
			clear_tokens_lst(&lexer->tokens);
		write(2, "error: missing quote\n", 22);
		return (1);
	}
	return (0);
}
