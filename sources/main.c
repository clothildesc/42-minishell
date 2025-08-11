/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:44:11 by cscache           #+#    #+#             */
/*   Updated: 2025/08/11 15:04:13 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	commande_test(char *input, t_shell *shell)
{
	int	i;

	if (ft_strncmp(input, "env", 3) == 0)
		ft_env(shell->env);
	if (ft_strncmp(input, "ast", 3) == 0)
		display_ast_results(&shell->ast, 0, ' ');
	if (ft_strncmp(input, "unset", 5) == 0)
	{
		i = 5;
		while (ft_isspace(input[i]))
			i++;
		ft_unset(&shell->env, &input[i]);
	}
	if (ft_strncmp(input, "export", 6) == 0)
	{
		i = 6;
		while (ft_isspace(input[i]))
			i++;
		ft_export(shell->env, &input[i]);
	}
}

/*
-	Peut-etre deplacer l'initialisation des struct dans le main
*/
int	execute_shell(char *input, t_env *env)
{
	t_token	*lst_tokens;
	t_shell	shell;

	init_all_structs(&shell);
	shell.env = env;
	lst_tokens = ft_lexer(input, &shell);
	shell.exit_status = get_syntax_error_status(lst_tokens);
	set_ast(&shell, lst_tokens);
	commande_test(input, &shell);
	return (shell.exit_status);
}

/*
- Declare les struct ici, reecupere l'env puis finalement readline
*/
int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_env	*env;

	(void)av;
	if (ac == 1)
	{
		env = get_env(envp);
		while (1)
		{
			line = readline("minishell> ");
			if (line == NULL)
				return (1);
			if (*line)
			{
				add_history(line);
				execute_shell(line, env);
				free(line);
			}
		}
		return (0);
	}
	return (-1);
}
