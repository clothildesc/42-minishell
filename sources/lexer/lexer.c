/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/09/08 16:32:59 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

static void	process_single_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '\'')
	{
		lexer->state = STATE_NORMAL;
		if (lexer->input[lexer->pos + 1] != ' ')
			create_token(lexer, true, true);
	}
	else
		add_char(&lexer->tmp_token, c);
}

static void	process_double_quote_state(t_lexer *lexer)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '"')
	{
		lexer->state = STATE_NORMAL;
		if (lexer->input[lexer->pos + 1] != ' ')
			create_token(lexer, true, false);
	}
	else
		add_char(&lexer->tmp_token, c);
}

static int	check_if_not_normal_state(t_lexer *lexer)
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

static void	process_current_char(t_lexer *lexer)
{
	if (lexer->state == STATE_NORMAL)
		process_normal_state(lexer);
	else if (lexer->state == STATE_SINGLE_QUOTE)
		process_single_quote_state(lexer);
	else if (lexer->state == STATE_DOUBLE_QUOTE)
		process_double_quote_state(lexer);
}

t_token	*ft_lexer(char *input, t_shell *shell)
{
	t_lexer		lexer;

	if (!shell || !input)
		return (NULL);
	ft_bzero(&lexer, sizeof(t_lexer));
	lexer.state = STATE_NORMAL;
	lexer.input = input;
	lexer.to_exp = true;
	lexer.to_join = false;
	lexer.pos = 0;
	while (lexer.input[lexer.pos])
	{
		process_current_char(&lexer);
		(lexer.pos)++;
	}
	if (check_if_not_normal_state(&lexer))
		return (NULL);
	create_token(&lexer, true, false);
	return (lexer.tokens);
}
