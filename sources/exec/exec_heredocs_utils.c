/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredocs_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 14:08:46 by cscache           #+#    #+#             */
/*   Updated: 2025/09/01 18:01:42 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

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

int	open_and_create_here_doc(char *tmp_file_name)
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

void	cleanup_heredoc_on_error(char *tmp_file_name, int fd_tmp, t_ast *root)
{
	g_signal_received = 130;
	close_prev_fd_heredoc(root);
	ft_close_fd(fd_tmp);
	unlink(tmp_file_name);
	free(tmp_file_name);
}
