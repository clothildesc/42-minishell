/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:34:28 by cscache           #+#    #+#             */
/*   Updated: 2025/09/05 09:56:06 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	free_child_and_exit(t_shell *shell, char **env_array, int exit_code)
{
	if (env_array)
		free_tab_chars(env_array);
	free_and_exit(shell, exit_code);
}
