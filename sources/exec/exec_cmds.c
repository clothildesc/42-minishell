/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:43:49 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 17:44:31 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

// static int	execute_child(t_cmd *cmd, char **env_array, t_shell *shell)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		return ;
// 	}
// 	if (pid == 0)
// 	{
// 		if (prepare_redirections(cmd) == -1)
// 			exit(EXIT_FAILURE);
// 		dup_with_pipe(shell->ast);
// 		execve(cmd->abs_path, cmd->args, env_array);
// 		perror("execve");
// 		exit(EXIT_CMD_NOT_FOUND);
// 	}
// 	cmd->pid = pid;
// }


// int execute_ast(ast , shell)
// {
// 	if(ast->type == PIPE)
// 	{	
// 		execute_ast(ast->left, shell);
// 		execute_ast(ast->right, shell);
// 	}
// 	if(ast->type == CMD)
// 		shell->exit_code = execute_cmd(ast, shell);
// }


// int	execute_ast(t_ast *node, t_shell *shell)
// {
// 	bool	first;
// 	int		pipefd[2];
// 	pid_t	pid;
// 	t_cmd	*cmd;
// 	char	**env_array;
// 	int		status;
// 	int		exit_code;


// 	exit_code = EXIT_SUCCESS;
// 	if (!node)
// 		return (EXIT_SUCCESS);
// 	if (pipe(pipefd) < 0)
// 		free_and_exit(shell, 1);
// 	env_array = lst_env_to_array(shell->env);
// 	if (!env_array)
// 	{
// 		perror("malloc env_array");
// 		return (EXIT_FAILURE);
// 	}
// 	first = true;
// 	if (!node)
// 		return (EXIT_FAILURE);
// 	if (node->node_type == NODE_PIPE)
// 	{
// 		execute_ast(node->data.binary.left, shell);
// 		execute_ast(node->data.binary.right, shell);
// 	}
// 	else if (node->node_type == NODE_CMD)
// 	{
// 		cmd = node->data.cmd.cmd;
// 		status = prepare_cmd(cmd, shell->env);
// 		if (status != EXIT_SUCCESS)
// 			return (status);
// 		if (first)
// 		{
// 			printf("first\n");
// 			pid = fork();
// 			if (pid == -1)
// 			{
// 				perror("fork");
// 				return (EXIT_FAILURE);
// 			}
// 			if (pid == 0)
// 			{
// 				printf("dans l'enfant\n");
// 				if (prepare_redirections(cmd) == -1)
// 					exit(EXIT_FAILURE);
// 				printf("avant de faire les dup\n");
// 				close(pipefd[0]);
// 				//if(redir in)
// 					dup2(node->data.cmd.cmd->fd_in, STDIN_FILENO);
// 				//sinon utiliser STDIN comme entree (cf la redir dans le parent)
// 				dup2(pipefd[1], STDOUT_FILENO);
// 				close(node->data.cmd.cmd->fd_in);
// 				close(pipefd[1]);
// 				first = false;
// 				printf("avant exec\n");
// 				execve(cmd->abs_path, cmd->args, env_array);
// 				printf("apres exec\n");
// 				perror("execve");
// 				exit(EXIT_CMD_NOT_FOUND);
// 			}
// 			cmd->pid = pid;
// 		}
// 		else
// 		{
// 			//rediriger fd[0] vers STDIN
// 			printf("too douuuux\n");
// 		}
// 		waitpid(cmd->pid, &status, 0);
// 		exit_code = get_exit_code(status);
// 		free_tab_chars(env_array);
// 	}
// 	return (exit_code);
// }
