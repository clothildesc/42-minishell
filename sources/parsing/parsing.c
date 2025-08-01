/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:07:21 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 18:44:23 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

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

void	create_args_lst(char *name, t_ast *ast)
{
	t_list	*new_arg;

	new_arg = malloc(sizeof(t_list));
	if (!new_arg)
		return ;
	new_arg->content = ft_strdup(name);
	if (!new_arg->content)
		return ;
	ft_lstadd_back(&ast->tmp_args, new_arg);
}

static void	free_args(char **result, int i)
{
	while (i >= 0)
	{
		if (result[i])
			free(result[i]);
		i--;
	}
	if (result[i])
		free(result[i]);
}

char	**convert_list_args_to_array(t_ast *ast)
{
	char	**args;
	t_list	*current;
	int		i;

	args = (char **)malloc(sizeof(char *) * ft_lstsize(ast->tmp_args));
	if (!args)
	{
		ft_lstclear(&ast->tmp_args, free);
		return (NULL);
	}
	i = 0;
	current = ast->tmp_args;
	while (current)
	{
		args[i] = ft_strdup((char *)current->content);
		if (!args[i])
		{
			free_args(args, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	args[i] = NULL;
	ft_lstclear(&ast->tmp_args, free);
	return (args);
}

static void	add_node_to_lst_cmds(t_cmd **lst, t_cmd *new)
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
	new->next = NULL;
}

static t_cmd *create_cmd_node(t_shell *shell, char *cmd_name)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->name = ft_strdup(cmd_name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	add_node_to_lst_cmds(&shell->ast.cmds, new);
	return (new);
}

void	set_ast(t_shell *shell, t_token *lst_tokens)
{
	t_token	*current;
	bool	first;
	t_cmd	*new;

	first = true;
	current = lst_tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			if (first)
			{
				new = create_cmd_node(shell, current->value);
				first = false;
			}
			else
				create_args_lst(current->value, &shell->ast);
		}
		else
		{
			if (shell->ast.tmp_args)
			{
				new->args = convert_list_args_to_array(&shell->ast);
				first = true;
			}
		}
		current = current->next;
	}
	if (shell->ast.tmp_args)
	{
		new->args = convert_list_args_to_array(&shell->ast);
		first = true;
	}
}
