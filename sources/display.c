/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:36:13 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/13 17:12:40 by cscache          ###   ########.fr       */
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
	int		i;
	t_arg	*current_arg;

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
		current_arg = cmd->args;
		i = 0;
		while (current_arg)
		{
			ft_printf("[%d]=%s ", i, current_arg->arg);
			current_arg = current_arg->next;
			i++;
		}
		ft_printf("\n");
	}
	print_indent(depth);
	if (cmd->fd_infile != -1)
		ft_printf("fd_infile: %d\n", cmd->fd_infile);
	else
		ft_printf("fd_infile: (null)\n");
	print_indent(depth);
	if (cmd->fd_outfile != -1)
		ft_printf("fd_outfile: %d\n", cmd->fd_outfile);
	else
		ft_printf("fd_outfile: (null)\n");
	ft_printf("\n");
}

/* void	display_ast_results(t_ast *ast, int depth, char branch)
{
	if (!ast)
		return ;
	ast = ast;
	print_indent(depth);
	if (branch != ' ')
		ft_printf("%c── ", branch);
	else
		ft_printf("    ");
	if (ast->node_type == NODE_PIPE)
		ft_printf("🔗 [PIPE]\n");
	else if (ast->node_type == NODE_CMD)
	{
		ft_printf("⚡ [CMD]\n");
		if (ast->cmds)
			print_cmd_node(ast->cmds, depth + 1);
	}
	if (ast->left)
		display_ast_results(ast->left, depth + 1, 'L');
	if (ast->right)
		display_ast_results(ast->right, depth + 1, 'R');
} */

void	display_ast_results(t_ast *ast, int depth, char branch)
{
	int	i;

	if (!ast)
		return ;
	i = 0;
	while (i < depth)
	{
		if (i == depth - 1)
			ft_printf("├── ");
		else
			ft_printf("│   ");
		i++;
	}
	if (depth > 0)
	{
		if (branch == 'L')
			ft_printf("LEFT: ");
		else if (branch == 'R')
			ft_printf("RIGHT: ");
	}
	if (ast->node_type == NODE_PIPE)
		ft_printf("[PIPE]\n");
	else if (ast->node_type == NODE_CMD)
	{
		if (ast->cmds)
		{
			ft_printf("[CMD] %s\n", ast->cmds->name);
			print_cmd_node(ast->cmds, depth + 1);
		}
		else
			ft_printf("[CMD] %s\n", "(null)");
	}
	if (ast->left)
		display_ast_results(ast->left, depth + 1, 'L');
	if (ast->right)
		display_ast_results(ast->right, depth + 1, 'R');
}
