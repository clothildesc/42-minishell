/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_up_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:23:31 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/29 17:25:15 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	ft_handler_sigint(int signum)
{
	g_signal_received = 128 + signum;
	write(STDOUT_FILENO, "^C", 2);
	if (!getenv("TERM"))
		write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_signal_received = 128 + signum;
	write(STDOUT_FILENO, "^C\n", 4);
	close(STDIN_FILENO);
}

void	set_up_signals_child(bool heredoc)
{
	if (heredoc)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}

void	set_up_signals_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
