/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 22:52:23 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	handle_normal_state(t_lexer *lexer, t_char_type char_type)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (char_type == CHAR_SINGLE_QUOTE)
		lexer->state = STATE_SINGLE_QUOTE;
	else if (char_type == CHAR_DOUBLE_QUOTE)
		lexer->state = STATE_DOUBLE_QUOTE;
	else if (char_type == CHAR_SPACE)
		create_token(lexer);
	else if (char_type == CHAR_PIPE || char_type == CHAR_REDIR_IN || char_type == CHAR_REDIR_OUT)
	{
		add_char(&lexer->tmp_token, c);
		if ((char_type == CHAR_REDIR_IN && lexer->input[lexer->pos + 1] == CHAR_REDIR_IN) ||
			(char_type == CHAR_REDIR_OUT && lexer->input[lexer->pos + 1] == CHAR_REDIR_OUT))
		{
			(lexer->pos)++;
			add_char(&lexer->tmp_token, lexer->input[lexer->pos]);
		}
		create_token(lexer);
	}
	else
		add_char(&lexer->tmp_token, c);
}

static void	handle_single_quote_state(t_lexer *lexer, t_char_type char_type)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (char_type == CHAR_SINGLE_QUOTE)
		lexer->state = STATE_NORMAL;
	else
		add_char(&lexer->tmp_token, c);
}

static void	handle_double_quote_state(t_lexer *lexer, t_char_type char_type)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (char_type == CHAR_DOUBLE_QUOTE)
		lexer->state = STATE_NORMAL;
	else
		add_char(&lexer->tmp_token, c);
}

static void	process_char(t_lexer *lexer, t_char_type char_type)
{
	if (lexer->state == STATE_NORMAL)
		handle_normal_state(lexer, char_type);
	else if (lexer->state == STATE_SINGLE_QUOTE)
		handle_single_quote_state(lexer, char_type);
	else if (lexer->state == STATE_DOUBLE_QUOTE)
		handle_double_quote_state(lexer, char_type);
}

t_token	*ft_lexer(char *input)
{
	t_lexer			lexer;
	t_char_type	char_type;

	init_struct_lexer(&lexer);
	lexer.input = input;
	while (lexer.input[lexer.pos])
	{
		char_type = classify_char_type(lexer.input[lexer.pos]);
		process_char(&lexer, char_type);
		(lexer.pos)++;
	}
	if (check_if_not_normal_state(&lexer))
		return (NULL); //implementer fonction qui attend une autre quote
	create_token(&lexer);
	return (lexer.tokens);
}
