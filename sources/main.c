/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 15:23:14 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char *av[])
{
	t_token	*tokens;

	if (ac < 2)
		return (1);
	tokens = ft_lexer(&av[1]);
	if (!tokens)
		return (1);
	while (tokens->next)
	{
		printf("%s\n", tokens->value);
		tokens = tokens->next;
	}
	clear_tokens_lst(&tokens);
	return (0);
}
