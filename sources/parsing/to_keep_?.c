/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_keep_?.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:26:58 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/15 21:02:37 by cscache          ###   ########.fr       */
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

// ----

// int	open_file(t_token *token, t_cmd *cmd)
// {
// 	if (token->type == TOKEN_REDIR_IN)
// 		fd = open_infile(token->next->value);
// 	else if (token->type == TOKEN_HERE_DOC)
// 		fd = create_here_doc(token->next->value);
// 	else if (token->type == TOKEN_REDIR_OUT)
// 		fd = open_outfile(token->next->value, token->type);
// 	else if (token->type == TOKEN_APPEND_OUT)
// 		fd = open_outfile(token->next->value, token->type);
//	return (fd);
// }