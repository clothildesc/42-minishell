/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_up_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:23:31 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/27 02:07:40 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	ft_handler(int signum)
{
	g_signal_recieved = 128 + signum;
	write(STDOUT_FILENO, "^C", 2);
	if (!getenv("TERM"))
		write(1, "\n", 1);
	close(0);
}

void	set_up_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
