/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:20:47 by cscache           #+#    #+#             */
/*   Updated: 2025/09/10 10:41:38 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	init_signals(void)
{
	rl_catch_signals = 0;
	set_up_signals_parent();
}

void	init_all_structs(t_shell *shell, char **envp)
{
	if (!shell)
		return ;
	ft_bzero(shell, sizeof(t_shell));
	shell->status = EXIT_SUCCESS;
	shell->env = get_env(envp);
}

void	init_shell(t_shell *shell, char **envp)
{
	init_all_structs(shell, envp);
	init_signals();
}
