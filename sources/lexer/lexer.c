/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 14:17:04 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	process_char(t_lexer *lexer)
{
	if (lexer->state == STATE_NORMAL)
		handle_normal_state(lexer);
	else if (lexer->state == STATE_SINGLE_QUOTE)
		handle_single_quote_state(lexer);
	else if (lexer->state == STATE_DOUBLE_QUOTE)
		handle_double_quote_state(lexer);
}

t_token	*ft_lexer(char *input, t_shell *shell)
{
	t_lexer		lexer;

	if (!shell || !input)
		return (NULL);
	lexer = shell->lexer;
	lexer.state = STATE_NORMAL;
	lexer.input = input;
	lexer.to_exp = true;
	lexer.to_join = 0;
	while (lexer.input[lexer.pos])
	{
		process_char(&lexer);
		(lexer.pos)++;
	}
	if (check_if_not_normal_state(&lexer))
		return (NULL);
	create_token(&lexer, true);
	return (lexer.tokens);
}
