/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/08/15 20:39:07 by cscache          ###   ########.fr       */
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
//	ATTENTION -> Si je fais un cat du heredoc et que j'ai des variables a expand
//	je dois bien les expand (ex : $USER)

static void	read_and_write_heredoc(int fd, char *limiter)
{
	char	*line;
	int		limiter_reached;

	limiter_reached = 0;
	write(1, "> ", 2);
	line = get_next_line(0);
	while (line)
	{
		write(1, "> ", 2);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			limiter_reached = 1;
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	if (!limiter_reached)
		ft_putendl_fd_no_nl("bash: warning: here-doc delimited by eof", 2);
	if (line)
		free(line);
}

int	create_here_doc(char *limiter)
{
	int		fd;

	fd = open("/tmp/.heredoc_tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("bash: open heredoc");
		return (-1);
	}
	read_and_write_heredoc(fd, limiter);
	close(fd);
	return (open("/tmp/.heredoc_tmp", O_RDONLY));
}

int	open_infile(char *infile)
{
	int	fd;

	if (access(infile, F_OK | R_OK) == -1)
	{
		ft_putendl_fd_no_nl("bash: ", 2);
		ft_putendl_fd_no_nl(infile, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (-1);
	}
	else
	{
		fd = open(infile, O_RDONLY);
		if (fd == -1)
		{
			ft_putendl_fd_no_nl("bash: ", 2);
			ft_putendl_fd_no_nl(infile, 2);
			ft_putendl_fd(": Permission denied", 2);
		}
	}
	return (fd);
}

int	open_outfile(char *outfile, t_token_type type)
{
	int	fd;

	if (type == TOKEN_APPEND_OUT)
		fd = open(outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
	else
	fd = open(outfile, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putendl_fd_no_nl("bash: ", 2);
		ft_putendl_fd_no_nl(outfile, 2);
		ft_putendl_fd(": Permission denied", 2);
	}
	return (fd);
}
