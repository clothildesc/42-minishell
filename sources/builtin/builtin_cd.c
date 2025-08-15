/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:38:45 by cscache           #+#    #+#             */
/*   Updated: 2025/08/15 20:06:49 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//verifier que ca fonctionne aussi sans args ->renvoie vers home

static int	args_lst_size(t_arg *args)
{
	int	size;

	size = 0;
	while (args)
	{
		size++;
		args = args->next;
	}
	return (size);
}

int	builtin_cd(t_arg *args)
{
	if (args_lst_size(args) > 1)
	{
		perror("cd: string not in pwd");
		return (EXIT_FAILURE);
	}
	if (chdir(args->arg) == -1)
	{
		perror(ERROR_CD);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
