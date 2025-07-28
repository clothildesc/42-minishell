/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:05:31 by cscache           #+#    #+#             */
/*   Updated: 2025/06/10 17:59:26 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"

int	is_newline(t_list *list)
{
	int	i;

	i = 0;
	if (!list)
		return (0);
	while (list->next)
		list = list->next;
	while (((char *)list->content)[i])
	{
		if (((char *)list->content)[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	read_and_create_list(t_list **list, int fd)
{
	char	*buffer;
	int		nb_read;
	t_list	*new_node;

	while (!is_newline(*list))
	{
		buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (!buffer)
			return ;
		nb_read = read(fd, buffer, BUFFER_SIZE);
		if (nb_read <= 0)
		{
			free(buffer);
			return ;
		}
		buffer[nb_read] = '\0';
		new_node = ft_lstnew(buffer);
		ft_lstadd_back(list, new_node);
	}
}
