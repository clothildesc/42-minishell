/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:36:13 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/11 18:15:15 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_lexer_results(t_token *lst_tokens)
{
	t_token	*head;

	if (!lst_tokens)
		return ;
	head = lst_tokens;
	while (lst_tokens)
	{
		ft_printf("VALUE: [%s]				TYPE: [%u]	TO_EXP: [%d]	TO_JOIN: [%d]\n", \
		lst_tokens->value, lst_tokens->type, \
		lst_tokens->to_exp, lst_tokens->to_join);
		lst_tokens = lst_tokens->next;
	}
	clear_tokens_lst(&head);
}

void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		ft_printf("    ");
		i++;
	}
}

void	print_cmd_node(t_cmd *cmd, int depth)
{
	t_cmd	*current;
	int		i;

	current = cmd;
	while (cmd)
	{
		print_indent(depth);
		if (cmd->name)
			ft_printf("Command: %s\n", cmd->name);
		else
			ft_printf("Command: (null)\n");

		print_indent(depth);
		if (cmd->args)
		{
			i = 0;
			while (cmd->args)
			{
				ft_printf("Args (%d) = %s\n", i, cmd->args->arg);
				print_indent(depth);
				i++;
				cmd->args = cmd->args->next;
			}
		}
		if (cmd->fds)
		{
			while (cmd->fds)
			{
				ft_printf("Redir '%s' vers %s\n", cmd->fds->redir, cmd->fds->file);
				print_indent(depth);
				cmd->fds = cmd->fds->next;
			}
		}
		ft_printf("\n");
		cmd = cmd->next;
	}
}

void	display_ast_results(t_ast *node, int depth, char branch)
{
	if (!node)
		return ;

	print_indent(depth);
	if (branch != ' ')
		ft_printf("%c── ", branch);
	else
		ft_printf("    ");
	if (node->node_type == NODE_PIPE)
		ft_printf("[PIPE]\n");
	else if (node->node_type == NODE_CMD)
	{
		ft_printf("[CMD]\n");
		print_cmd_node(node->cmds, depth + 1);
	}

	if (node->left)
		display_ast_results(node->left, depth + 1, 'L');
	if (node->right)
		display_ast_results(node->right, depth + 1, 'R');
}
