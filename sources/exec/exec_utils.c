/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:40:22 by cscache           #+#    #+#             */
/*   Updated: 2025/09/03 09:00:34 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	env_lst_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

static char	**fill_env_array(char **env_array, t_env *env)
{
	int		i;
	t_env	*current;
	char	*tmp;

	i = 0;
	current = env;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		if (!tmp)
			return (NULL);
		env_array[i] = ft_strjoin(tmp, current->value);
		if (!env_array[i])
		{
			free(tmp);
			free_tab_chars(env_array);
			return (NULL);
		}
		free(tmp);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	**lst_env_to_array(t_env *env)
{
	char	**env_array;

	env_array = (char **)malloc(sizeof(char *) * (env_lst_size(env) + 1));
	if (!env_array)
		return (NULL);
	fill_env_array(env_array, env);
	if (!env_array)
	{
		free(env_array);
		return (NULL);
	}
	return (env_array);
}

void	close_all_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	while (i != shell->nb_pipes)
	{
		ft_close_fd(shell->pipes[i][0]);
		ft_close_fd(shell->pipes[i][1]);
		i++;
	}
}

void	close_all_command_fds(t_ast *node)
{
	t_cmd	*cmd;

	cmd = node->data.cmd.cmd;
	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
	{
		close_all_command_fds(node->data.binary.left);
		close_all_command_fds(node->data.binary.right);
	}
	else if (node->node_type == NODE_CMD)
	{
		if (cmd)
		{
			ft_close_fd(cmd->fd_in);
			ft_close_fd(cmd->fd_out);
			ft_close_fd(cmd->fd_heredoc);
		}
	}
}
