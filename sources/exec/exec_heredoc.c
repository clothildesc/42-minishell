/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/08/28 16:45:08 by cscache          ###   ########.fr       */
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

static void	read_and_write_heredoc(int fd, char *limiter)
{
	char	*line;
	int		limiter_reached;

	limiter_reached = 0;
	remove_echoctl();
	while (true)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (g_signal_received == 130)
		{
			free(line);
			return ;
		}
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 \
			&& line[ft_strlen(limiter)] == '\n')
		{
			limiter_reached = 1;
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	active_echoctl();
	if (!limiter_reached)
		ft_putendl_fd("\nbash: warning: here-doc delimited by eof", 2);
}

int	get_unique_id(void)
{
	static int	counter;

	counter++;
	return (counter);
}

char	*get_file_name(void)
{
	int		unique_id;
	char	*tmp_file_name;
	char	*id_str;

	unique_id = get_unique_id();
	id_str = ft_itoa(unique_id);
	if (!id_str)
		return (NULL);
	tmp_file_name = ft_strjoin("/tmp/.heredoc_minishell_", id_str);
	free(id_str);
	if (!tmp_file_name)
		return (NULL);
	printf("filename: %s\n", tmp_file_name);
	return (tmp_file_name);
}

static int	create_here_doc(char *limiter, char *tmp_file_name)
{
	int		fd;

	fd = open(tmp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	printf("fd: %d\n", fd);
	if (fd == -1)
	{
		perror("bash: open heredoc");
		free(tmp_file_name);
		return (-1);
	}
	read_and_write_heredoc(fd, limiter);
	close(fd);
	fd = open(tmp_file_name, O_RDONLY);
	unlink(tmp_file_name);
	return (free(tmp_file_name), fd);
}

void	close_prev_fd_heredoc(t_ast *node)
{
	t_cmd	*cmd;

	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
	{
		close_prev_fd_heredoc(node->data.binary.left);
		close_prev_fd_heredoc(node->data.binary.right);
	}
	else if (node->node_type == NODE_CMD)
	{
		cmd = node->data.cmd.cmd;
		if (cmd->fd_heredoc != -1)
			close(cmd->fd_heredoc);
	}
}

static void	execute_child_heredoc(t_ast *root, t_cmd *cmd, char *limiter, int fd_heredoc)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork heredoc");
		return ;
	}
	if (pid == 0)
	{
		close_prev_fd_heredoc(root);
		set_up_signals_child(true);
		read_and_write_heredoc(fd_heredoc, limiter);
		if (g_signal_received)
			exit(g_signal_received);
		else
			exit(EXIT_SUCCESS);
	}
	cmd->pid_heredoc = pid;
}

static int	process_heredoc(t_ast *root, t_cmd *cmd, char *target)
{
	int		status;
	int		exit_code;
	int		fd_tmp;
	char	*tmp_file_name;

	if (cmd->fd_heredoc != -1)
		close(cmd->fd_heredoc);
	tmp_file_name = get_file_name();
	if (!tmp_file_name)
		return (EXIT_FAILURE);
	fd_tmp = create_here_doc(target, tmp_file_name);
	if (fd_tmp == -1)
		return (EXIT_FAILURE);
	execute_child_heredoc(root, cmd, target, cmd->fd_heredoc);
	waitpid(cmd->pid_heredoc, &status, 0);
	printf("pid_heredoc: %d\n", cmd->pid_heredoc);
	exit_code = get_exit_code(status);
	close(fd_tmp);
	cmd->fd_heredoc = open(tmp_file_name, O_RDONLY);
	if (cmd->fd_heredoc == -1)
		return (EXIT_FAILURE);
	printf("fd_heredoc: %d\n", cmd->fd_heredoc);
	unlink(tmp_file_name);
	return (exit_code);
}

void	handle_all_heredocs(t_ast *node, t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*current_redir;

	if (!node)
		return ;
	if (node->node_type == NODE_PIPE)
	{
		handle_all_heredocs(node->data.binary.left, shell);
		handle_all_heredocs(node->data.binary.right, shell);
	}
	else if (node->node_type == NODE_CMD)
	{
		cmd = node->data.cmd.cmd;
		current_redir = cmd->redirs;
		while (current_redir)
		{
			if (current_redir->type == HERE_DOC)
				shell->status = process_heredoc(shell->ast, cmd, \
					current_redir->target);
			current_redir = current_redir->next;
		}
	}
}
