/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/30 15:11:33 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	handle_normal_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
		lexer->state = STATE_SINGLE_QUOTE;
	else if (c == '"')
		lexer->state = STATE_DOUBLE_QUOTE;
	else if (c == ' ')
		create_token(lexer);
	else if (c == '|' || c == '<' || c == '>')
	{
		add_char(&lexer->tmp_token, c);
		if ((c == '<' && lexer->input[lexer->pos + 1] == '<') || \
		(c == '>' && lexer->input[lexer->pos + 1] == '>'))
		{
			(lexer->pos)++;
			add_char(&lexer->tmp_token, lexer->input[lexer->pos]);
		}
		create_token(lexer);
	}
	else
		add_char(&lexer->tmp_token, c);
}

static void	handle_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
	{
		lexer->state = STATE_NORMAL;
		lexer->single_quote = 1;
	}
	else if (c == '"')
	{
		lexer->state = STATE_NORMAL;
		lexer->double_quote = 1;
	}
	else
		add_char(&lexer->tmp_token, c);
}

static void	process_char(t_lexer *lexer)
{
	if (lexer->state == STATE_NORMAL)
		handle_normal_state(lexer);
	else if (lexer->state == STATE_SINGLE_QUOTE)
		handle_quote_state(lexer);
	else if (lexer->state == STATE_DOUBLE_QUOTE)
		handle_quote_state(lexer);
}

t_token	*ft_lexer(char *input, t_shell *shell)
{
	t_lexer		lexer;

	lexer = shell->lexer;
	lexer.state = STATE_NORMAL;
	lexer.input = input;
	while (lexer.input[lexer.pos])
	{
		process_char(&lexer);
		(lexer.pos)++;
	}
	if (check_if_not_normal_state(&lexer))
		return (NULL); //implementer fonction qui attend une autre quote
	create_token(&lexer);
	return (lexer.tokens);
}

