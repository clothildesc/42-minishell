/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_up_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:23:31 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/27 10:52:24 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	ft_signal_handler(int signum)
{
	g_signal_received = 128 + signum;
	write(STDOUT_FILENO, "^C", 2);
	if (!getenv("TERM"))
		write(1, "\n", 1);
	close(EXIT_SUCCESS);
}

void	set_up_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
