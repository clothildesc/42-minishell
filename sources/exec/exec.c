/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/14 18:30:10 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_a_builtin(char *cmd_name)
{
	if (ft_strcmp(cmd_name, "env") == 0)
		return (1);
	// else if (ft_strcmp(cmd_name, "pwd") == 0)
	// 	return (1);
	// else if (ft_strcmp(cmd_name, "cd") == 0)
	// 	return (1);
	// else if (ft_strcmp(cmd_name, "unset") == 0)
	// 	return (1);
	else if (ft_strcmp(cmd_name, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd_name, "echo") == 0)
		return (1);
	else
		return (0);
}

/* Il va falloir la modifier un peu quand on l'incluera dans l'exec complete */
int	exec_builtins(t_shell *shell)
{
	t_ast	*current;

	if (!shell)
		return (EXIT_FAILURE);
	current = shell->ast;
	while (current)
	{
		if (is_a_builtin(current->cmds->name))
		{
			if (ft_strcmp(current->cmds->name, "env") == 0)
				return (builtin_env(shell->env));
			// else if (ft_strcmp(current->cmds->name, "pwd") == 0)
			// 	return (builtin_pwd());
			else if (ft_strcmp(current->cmds->name, "echo") == 0)
				return (builtin_echo(current->cmds->args));
			// else if (ft_strcmp(current->cmds->name, "cd") == 0)
			// 	return (builtin_cd(current->cmds->args));
			// else if (ft_strcmp(current->cmds->name, "unset") == 0)
			// 	return (builtin_unset(&shell->env, current->cmds->args));
			else if (ft_strcmp(current->cmds->name, "export") == 0)
				return (builtin_export(shell->env, current->cmds->args));
		}
		current = current->left;
	}
	return (EXIT_SUCCESS);
}
