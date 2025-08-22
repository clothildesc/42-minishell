/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/08/22 11:46:08 by cscache          ###   ########.fr       */
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
	while (1)
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
	if (!limiter_reached)
		ft_putstr_fd("bash: warning: here-doc delimited by eof", 2);
}

static int	get_unique_id(void)
{
	static int	counter;

	counter++;
	return (counter);
}

static int	create_here_doc(char *limiter)
{
	int		fd;
	int		unique_id;
	char	*tmp_file_name;
	char	*id_str;

	unique_id = get_unique_id();
	id_str = ft_itoa(unique_id);
	if (!id_str)
		return (-1);
	tmp_file_name = ft_strjoin("/tmp/.heredoc_", id_str);
	free(id_str);
	if (!tmp_file_name)
		return (-1);
	fd = open(tmp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

static int	process_heredoc(t_cmd *cmd, char *target)
{
	if (cmd->fd_heredoc != -1)
		close(cmd->fd_heredoc);
	cmd->fd_heredoc = create_here_doc(target);
	if (cmd->fd_heredoc == -1)
		return (-1);
	return (0);
}

int	handle_all_heredocs(t_ast *node)
{
	t_cmd	*cmd;
	t_redir	*current_redir;

	if (!node)
		return (0);
	if (node->node_type == NODE_PIPE)
	{
		handle_all_heredocs(node->data.binary.left);
		handle_all_heredocs(node->data.binary.right);
	}
	else if (node->node_type == NODE_CMD)
	{
		cmd = node->data.cmd.cmd;
		current_redir = cmd->redirs;
		while (current_redir)
		{
			if (current_redir->type == TOKEN_HERE_DOC)
				if (process_heredoc(cmd, current_redir->target) == -1)
					return (-1);
			current_redir = current_redir->next;
		}
	}
	return (0);
}
