/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:44:26 by cscache           #+#    #+#             */
/*   Updated: 2025/08/12 18:09:33 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

// HERE-DOC
// Créer un fichier temporaire pour stocker le contenu du here_doc
// Lire l'input utilisateur ligne par ligne jusqu'au délimiteur
// Écrire dans le fichier temporaire
// Retourner un fd en lecture sur ce fichier temporaire

void	read_and_write_heredoc(int fd, char *limiter)
{
	char	*line;
	int		limiter_reached;

	limiter_reached = 0;
	write(1, "> ", 1);
	line = get_next_line(0);
	while (line)
	{
		write(1, "> ", 1);
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
		write(2, "[minishell] warning: there is no limiter\n", 42);
	if (line)
		free(line);
}

int	create_here_doc(char *limiter)
{
	int		fd;
	char	*line;

	fd = open(0, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (-1);
	read_and_write_heredoc(fd, limiter);
	return (fd);
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
