/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 16:20:08 by cscache           #+#    #+#             */
/*   Updated: 2025/08/15 21:06:53 by cscache          ###   ########.fr       */
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

void	traverse_ast(t_ast *node)
{
	
}

/* Il va falloir la modifier un peu quand on l'incluera dans l'exec complete */
int	exec_builtins(t_shell *shell)
{
	t_ast	*current;

	current = shell->ast;
	while (current)
	{
		if (is_a_builtin(current->data.cmd.cmd->name))
		{
			if (ft_strcmp(current->data.cmd.cmd->name, "env") == 0)
				return (builtin_env(shell->env));
			// else if (ft_strcmp(current->data.cmd.cmd->name, "pwd") == 0)
			// 	return (builtin_pwd());
			else if (ft_strcmp(current->data.cmd.cmd->name, "echo") == 0)
				return (builtin_echo(current->data.cmd.cmd->args));
			// else if (ft_strcmp(current->data.cmd.cmd->name, "cd") == 0)
			// 	return (builtin_cd(current->data.cmd.cmd->args));
			// else if (ft_strcmp(current->data.cmd.cmd->name, "unset") == 0)
			// 	return (builtin_unset(&shell->env, current->data.cmd.cmd->args));
			else if (ft_strcmp(current->data.cmd.cmd->name, "export") == 0)
				return (builtin_export(shell->env, current->data.cmd.cmd->args));
		}
		current = current->left;
	}
	return (EXIT_SUCCESS);
}
