/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/14 11:11:50 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	commande_test(char *input, t_shell *shell)
{
	int	i;

	if (ft_strncmp(input, "env", 3) == 0)
		builtin_env(shell->env);
	if (ft_strncmp(input, "pwd", 3) == 0)
		builtin_pwd();
	if (ft_strncmp(input, "ast", 3) == 0)
		display_ast_results(shell->ast, 0, ' ');
	if (ft_strncmp(input, "cd", 2) == 0)
	{
		i = 2;
		while (ft_isspace(input[i]))
			i++;
		builtin_cd(&input[i]);
	}
	if (ft_strncmp(input, "unset", 5) == 0)
	{
		i = 5;
		while (ft_isspace(input[i]))
			i++;
		builtin_unset(&shell->env, &input[i]);
	}
	if (ft_strncmp(input, "export", 6) == 0)
	{
		i = 6;
		while (ft_isspace(input[i]))
			i++;
		builtin_export(shell->env, &input[i]);
	}
}

/*
-	Peut-etre deplacer l'initialisation des struct dans le main
*/
int	execute_shell(char *input, t_shell *shell)
{
	shell->tokens = NULL;
	shell->ast = NULL;

	shell->tokens = ft_lexer(input, shell);
	shell->exit_status = get_syntax_error_status(shell->tokens);
	//display_lexer_results(shell->tokens);
	shell->ast = set_ast(shell, &shell->tokens);
	commande_test(input, shell);
	clear_tokens_lst(&shell->tokens);
	clear_ast(&shell->ast);
	return (shell->exit_status);
}

/*
- Declare les struct ici, reecupere l'env puis finalement readline
*/
int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_shell	shell;

	(void)av;
	if (ac == 1)
	{
		init_all_structs(&shell);
		shell.env = get_env(envp);
		while (1)
		{
			line = readline("minishell> ");
			if (line == NULL)
			{
				clear_env_lst(&shell.env);
				return (1);
			}
			if (*line)
			{
				add_history(line);
				execute_shell(line, &shell);
			}
			free(line);
		}
		clear_env_lst(&shell.env);
		return (0);
	}
	return (-1);
}
