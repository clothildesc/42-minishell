/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/08/29 17:38:17 by barmarti         ###   ########.fr       */
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
	if (!limiter_reached && g_signal_received != 130)
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
	return (tmp_file_name);
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
		write(1, "dans l'enfant\n", 15);
		close_prev_fd_heredoc(root);
		set_up_signals_child(true);
		read_and_write_heredoc(fd_heredoc, limiter);
		if (fd_heredoc != -1)
		{
			write(1, "close dans l'enfant de fd_tmp\n", 31);
			close(fd_heredoc);
		}
		if (g_signal_received)
		{
			write(1, "g_signal_received\n", 19);
			exit(g_signal_received);
		}
		else
			exit(EXIT_SUCCESS);
	}
	cmd->pid_heredoc = pid;
}

static int	open_and_create_here_doc(char *tmp_file_name)
{
	int		fd;

	fd = open(tmp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	printf("fd: %d\n", fd);
	if (fd == -1)
	{
		perror("bash: open heredoc");
		free(tmp_file_name);
		return (fd);
	}
	return (fd);
}

void	close_prev_fd_heredoc(t_ast *node)
{
	t_cmd	*cmd;

	printf("dans close prev heredocs\n");
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
		{
			printf("close fd_heredoc de cmd precedente\n");
			close(cmd->fd_heredoc);
		}
	}
}

static int	process_heredoc(t_ast *root, t_cmd *cmd, char *target)
{
	int		status;
	int		exit_code;
	int		fd_tmp;
	char	*tmp_file_name;

	if (cmd->fd_heredoc != -1)
	{
		printf("close fd_heredoc existant\n");
		close(cmd->fd_heredoc);
	}
	tmp_file_name = get_file_name();
	if (!tmp_file_name)
		return (EXIT_FAILURE);
	fd_tmp = open_and_create_here_doc(tmp_file_name);
	printf("fd_tmp: %d\n", fd_tmp);
	if (fd_tmp == -1)
		return (free(tmp_file_name), unlink(tmp_file_name), EXIT_FAILURE);
	execute_child_heredoc(root, cmd, target, fd_tmp);
	waitpid(cmd->pid_heredoc, &status, 0);
	exit_code = get_exit_code(status);
	if (exit_code == 130)
	{
		printf("dans exit_code 130\n");
		g_signal_received = 130;
		close_prev_fd_heredoc(root);
		if (fd_tmp != -1)
		{
			printf("close fd_tmp dans exit_code 130\n");
			close(fd_tmp);
		}
		unlink(tmp_file_name);
		free(tmp_file_name);
		return (exit_code);
	}
	if (fd_tmp != -1)
	{
		printf("close fd_tmp dans le parent\n");
		close(fd_tmp);
	}
	cmd->fd_heredoc = open(tmp_file_name, O_RDONLY);
	if (cmd->fd_heredoc == -1)
		return (unlink(tmp_file_name), free(tmp_file_name), EXIT_FAILURE);
	unlink(tmp_file_name);
	free(tmp_file_name);
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
		if (shell->status == 130)
			return ;
		handle_all_heredocs(node->data.binary.right, shell);
	}
	else if (node->node_type == NODE_CMD)
	{
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
}
