/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/09/01 18:02:59 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

// HERE-DOC
// Créer un fichier temporaire pour stocker le contenu du here_doc
// Lire l'input utilisateur ligne par ligne jusqu'au délimiteur
// Écrire dans le fichier temporaire
// Retourner un fd en lecture sur ce fichier temporaire

// ne pas oublier de unlink le heredoc ensuite : unlink("/tmp/.heredoc_tmp")
// (optionnel) -> expand dans le heredoc
//	idealement il faudrait que je les expand (ex : $USER)

static int	open_heredoc_read_only(char *tmp_file_name, t_cmd *cmd)
{
	cmd->fd_heredoc = open(tmp_file_name, O_RDONLY);
	if (cmd->fd_heredoc == -1)
		return (unlink(tmp_file_name), free(tmp_file_name), EXIT_FAILURE);
	unlink(tmp_file_name);
	free(tmp_file_name);
	return (EXIT_SUCCESS);
}

static int	process_heredoc(t_ast *root, t_cmd *cmd, char *target)
{
	int		status;
	int		exit_code;
	int		fd_tmp;
	char	*tmp_file_name;

	ft_close_fd(cmd->fd_heredoc);
	tmp_file_name = get_file_name();
	if (!tmp_file_name)
		return (EXIT_FAILURE);
	fd_tmp = open_and_create_here_doc(tmp_file_name);
	if (fd_tmp == -1)
		return (free(tmp_file_name), unlink(tmp_file_name), EXIT_FAILURE);
	execute_child_heredoc(root, cmd, target, fd_tmp);
	waitpid(cmd->pid_heredoc, &status, 0);
	exit_code = get_exit_code(status);
	if (exit_code == 130)
		return (cleanup_heredoc_on_error(tmp_file_name, fd_tmp, root), 130);
	ft_close_fd(fd_tmp);
	return (open_heredoc_read_only(tmp_file_name, cmd));
}

static void	handle_pipe_heredocs(t_ast *node, t_shell *shell)
{
	handle_all_heredocs(node->data.binary.left, shell);
	if (shell->status == 130)
		return ;
	handle_all_heredocs(node->data.binary.right, shell);
}

static void	handle_cmd_heredocs(t_ast *node, t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*current_redir;

	cmd = node->data.cmd.cmd;
	current_redir = cmd->redirs;
	while (current_redir)
	{
		if (current_redir->type == HERE_DOC)
		{
			shell->status = process_heredoc(shell->ast, cmd, \
				current_redir->target);
			if (shell->status == 130)
				return ;
		}
		current_redir = current_redir->next;
	}
}

void	handle_all_heredocs(t_ast *node, t_shell *shell)
{
	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
		handle_pipe_heredocs(node, shell);
	else if (node->node_type == NODE_CMD)
		handle_cmd_heredocs(node, shell);
}
