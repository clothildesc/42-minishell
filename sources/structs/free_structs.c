/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:43:01 by cscache           #+#    #+#             */
/*   Updated: 2025/08/28 18:59:24 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	clear_ast(t_ast **ast)
{
	if (!ast || !*ast)
		return ;
	if ((*ast)->node_type == NODE_PIPE)
	{
		clear_ast(&(*ast)->data.binary.left);
		clear_ast(&(*ast)->data.binary.right);
	}
	else if ((*ast)->node_type == NODE_CMD)
	{
		if ((*ast)->data.cmd.cmd)
		{
			clear_cmd((*ast)->data.cmd.cmd);
			free((*ast)->data.cmd.cmd);
		}
	}
	free(*ast);
	*ast = NULL;
}

void	clear_env_lst(t_env **env)
{
	t_env	*tmp;

	if (!env || !*env)
		return ;
	while (*env)
	{	
		tmp = *env;
		*env = (*env)->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	clear_lexer_tmp(t_lexer *lexer)
{
	if (lexer && lexer->tmp_token)
	{
		ft_lstclear(&lexer->tmp_token, free);
		lexer->tmp_token = NULL;
	}
}

void	free_tab_pids(t_shell *shell, pid_t *pids)
{
	int	len;
	int	i;

	len = shell->nb_cmds;
	i = 0;
	if (!pids)
		return ;
	while (i < len)
	{
		free(&pids[i]);
		i++;
	}
	free(pids);
	pids = NULL;
}

void	clear_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->tokens)
		clear_tokens_lst(&shell->tokens);
	if (shell->ast)
		clear_ast(&shell->ast);
	if (shell->pids)
		free_tab_pids(shell, shell->pids);
	if (shell->env)
		clear_env_lst(&shell->env);
	clear_lexer_tmp(&shell->lexer);
}

void	free_and_exit(t_shell *shell, int exit_code)
{
	if (!shell)
		return ;
	clear_shell(shell);
	exit(exit_code);
}
