/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_keep_?.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:26:58 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/12 17:57:19 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// static void	add_node_to_lst_cmds(t_cmd **lst, t_cmd *new)
// {
// 	t_cmd	*last;

// 	last = NULL;
// 	if (!lst || !new)
// 		return ;
// 	if (*lst)
// 	{
// 		last = *lst;
// 		while (last->next)
// 			last = last->next;create_redir_lst(current, new->cmds->fds);
// 		new->prev = last;
// 		last->next = new;
// 	}
// 	else
// 		*lst = new;
// 	new->next = NULL;
// }

// char	**convert_list_args_to_array(t_cmd *cmd)
// {
// 	char	**args;
// 	t_list	*current;
// 	int		i;

// 	args = (char **)malloc(sizeof(char *) * (ft_lstsize(cmd->tmp_args) + 1));
// 	if (!args)
// 	{
// 		ft_lstclear(&cmd->tmp_args, free);
// 		return (NULL);
// 	}
// 	i = 0;
// 	current = cmd->tmp_args;
// 	while (current)
// 	{
// 		args[i++] = ft_strdup((char *)current->content);
// 		if (!args[i - 1])
// 		{
// 			free_args(args, (i - 1));
// 			return (NULL);
// 		}
// 		current = current->next;
// 	}
// 	args[i] = NULL;
// 	ft_lstclear(&cmd->tmp_args, free);
// 	return (args);
// }

/* static void	add_to_lst_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	last = NULL;
	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = *lst;
		while (last->next)
			last = last->next;
		new->prev = last;
		last->next = new;
	}
	else
		*lst = new;
} */

/* void	ft_lstadd_redir(t_redir **lst, t_redir *new)
{
	t_redir	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
		}
		else
			*lst = new;
	}
}

void	create_redir_lst(t_token *token, t_cmd *cmd)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	ft_bzero(new_redir, sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->file = ft_strdup(token->next->value);
	if (!new_redir->file)
	{
		free(new_redir);
		return ;
	}
	new_redir->redir = ft_strdup(token->value);
	if (!new_redir->redir)
	{
		free(new_redir);
		return ;
	}
	ft_lstadd_redir(&cmd->fds, new_redir);
} */