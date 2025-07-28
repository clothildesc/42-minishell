/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/25 17:44:01 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "ft_lexer.h"

void	init_struct_lexer(t_lexer *l)
{
	l->state = STATE_NORMAL;
	l->tmp_token = NULL;
	l->lst_tokens = NULL;
	l->input = NULL;
	l->pos = 0;
}

char	**ft_lexer(char **input)
{
	t_lexer		lexer;
	t_char_type	type;
	int			i;

	init_struct_lexer(&lexer);
	lexer.input = input;
	while (lexer.input[i])
	{
		while (lexer.input[i][lexer.pos])
		{
			type = classify_char_type(lexer.input[lexer.pos]);
			if (type == CHAR_SINGLE_QUOTE)
				handle_single_quote_state(&lexer);
			else if (type == CHAR_DOUBLE_QUOTE)
				handle_double_quote_state(&lexer);
			else if (type == CHAR_SPACE)
				handle_space_state(&lexer);
			else
				handle_normal_state(&lexer);
			(lexer.pos)++;
		}
		i++;
	}
	if (check_if_not_normal_state(&lexer))
		return (NULL);
	finish_token(&(lexer.tmp_token), &(lexer.lst_tokens));
	return (lst_to_array(lexer.lst_tokens));
}

#include <stdio.h>

int	main(int ac, char *av[])
{
	char	**array;
	int		i;

	i = 0;
	if (ac == 2)
	{
		array = ft_lexer(&av[1]);
		if (!array)
			return (1);
		while (array[i])
		{
			printf("%s\n", array[i]);
			free(array[i]);
			i++;
		}
		free(array);
	}
	return (0);
}
