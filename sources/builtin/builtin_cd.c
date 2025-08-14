/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:38:45 by cscache           #+#    #+#             */
/*   Updated: 2025/08/13 11:25:14 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_cd(char *path)
{
	if (chdir(path) == -1)
	{
		perror("bash: cd: x: No such file or directory");
		return (1);
	}
	return (0);
}
