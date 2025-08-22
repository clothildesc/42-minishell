/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:36:13 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/19 15:10:00 by cscache          ###   ########.fr       */
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
		ft_printf("VALUE: [%s]	TYPE: [%u]	TO_EXP: [%d]	TO_JOIN: [%d]\n", \
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
	int		i;

	if (!cmd)
		return ;
	print_indent(depth);
	if (cmd->name)
		ft_printf("Command: %s\n", cmd->name);
	else
		ft_printf("Command: (null)\n");
	if (cmd->args)
	{
		print_indent(depth);
		ft_printf("Args: ");
		i = 0;
		while (cmd->args[i])
		{
			ft_printf("[%d]= %s ", i, cmd->args[i]);
			i++;
		}
		ft_printf("\n");
	}
}

void	display_ast_results(t_ast *ast, int depth, char branch)
{
	if (!ast)
		return ;
	print_indent(depth);
	if (branch != ' ')
		ft_printf("%c── ", branch);
	else
		ft_printf("    ");
	if (ast->node_type == NODE_PIPE)
		ft_printf("[PIPE]\n");
	else if (ast->node_type == NODE_CMD)
	{
		ft_printf("[CMD]\n");
		if (ast->data.cmd.cmd)
			print_cmd_node(ast->data.cmd.cmd, depth + 1);
	}
	if (ast->data.binary.left)
		display_ast_results(ast->data.binary.left, depth + 1, 'L');
	if (ast->data.binary.right)
		display_ast_results(ast->data.binary.right, depth + 1, 'R');
}
