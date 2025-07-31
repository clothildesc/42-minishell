/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/31 18:04:13 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	set_to_join(t_lexer *lexer)
{
	if (lexer->input[lexer->pos] != ' ')
		lexer->to_join = 1;
}

static void	handle_normal_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
	{
		if (lexer->state == STATE_NORMAL && lexer->tmp_token)
		{
			set_to_join(lexer);
			create_token(lexer);
		}
		lexer->state = STATE_SINGLE_QUOTE;
		if (lexer->input[lexer->pos + 1] == '$')
			lexer->to_exp = false;
	}
	else if (c == '"')
	{
		if (lexer->state == STATE_NORMAL && lexer->tmp_token)
		{
			set_to_join(lexer);
			create_token(lexer);
		}
		lexer->state = STATE_DOUBLE_QUOTE;
	}
	else if (c == ' ')
	{
		set_to_join(lexer);
		create_token(lexer);
	}
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

static void	handle_single_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
	{
		lexer->state = STATE_NORMAL;
		if (lexer->input[lexer->pos + 1] != 0)
		{
			set_to_join(lexer);
			create_token(lexer);
		}
	}
	else
		add_char(&lexer->tmp_token, c);
}

static void	handle_double_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '"')
	{
		lexer->state = STATE_NORMAL;
		{
			set_to_join(lexer);
			create_token(lexer);
		}
	}
	else
		add_char(&lexer->tmp_token, c);
}

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
		return (NULL); //implementer fonction qui attend une autre quote
	set_to_join(&lexer);	
	create_token(&lexer);
	return (lexer.tokens);
}

