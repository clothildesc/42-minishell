/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 11:58:04 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_builtin_simple(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;
	int	saved_in;
	int	saved_out;

	exit_code = EXIT_SUCCESS;
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
		return (EXIT_FAILURE);
	if (prepare_redirections(cmd) == -1)
	{
		close(saved_in);
		close(saved_out);
		return (EXIT_FAILURE);
	}
	apply_redirections(cmd);
	exit_code = execute_builtins(cmd, shell);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (exit_code);
}

int	exec_builtin_in_parent(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;
	int	saved_in;
	int	saved_out;

	exit_code = EXIT_SUCCESS;
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
		return (EXIT_FAILURE);
	if (prepare_redirections(cmd) == -1)
	{
		close(saved_in);
		close(saved_out);
		return (EXIT_FAILURE);
	}
	apply_redirections(cmd);
	exit_code = execute_parent_builtins(cmd, shell);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (exit_code);
}
