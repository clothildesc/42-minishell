/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:05:13 by cscache           #+#    #+#             */
/*   Updated: 2025/06/10 17:59:18 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"

static void	extract_line(t_list *list, char *line)
{
	int		i;
	int		j;

	j = 0;
	if (!list)
		return ;
	while (list)
	{
		i = 0;
		while (((char *)list->content)[i])
		{
			if (((char *)list->content)[i] == '\n')
			{
				line[j++] = '\n';
				line[j] = '\0';
				return ;
			}
			line[j++] = ((char *)list->content)[i++];
		}
		list = list->next;
	}
	line[j] = '\0';
}

static int	line_length(t_list **list)
{
	int		i;
	int		len;
	t_list	*temp;

	len = 0;
	if (!list)
		return (0);
	temp = *list;
	while (temp)
	{
		i = 0;
		while (((char *)temp->content)[i])
		{
			len++;
			if (((char *)temp->content)[i] == '\n')
				return (len);
			i++;
		}
		temp = temp->next;
	}
	return (len);
}

static void	free_and_set__next_list(t_list **list, t_list *new_node, char *rest)
{
	t_list	*temp;

	if (!*list)
		return ;
	while (*list)
	{
		temp = (*list)->next;
		free((*list)->content);
		free(*list);
		*list = temp;
	}
	if (!new_node)
	{
		free(rest);
		*list = NULL;
	}
	else
		*list = new_node;
}

static void	clean_list(t_list **list)
{
	t_list	*last_node;
	t_list	*new_node;
	char	*rest;
	int		i;

	i = 0;
	new_node = NULL;
	rest = NULL;
	if (!list || !*list)
		return ;
	last_node = *list;
	while (last_node->next)
	last_node = last_node->next;
	while (((char *)last_node->content)[i] && \
	((char *)last_node->content)[i] != '\n')
	i++;
	if (((char *)last_node->content)[i] == '\n' \
	&& ((char *)last_node->content)[i + 1])
	{
		i++;
		rest = ft_strdup(((char *)last_node->content) + i);
		new_node = ft_lstnew(rest);
	}
	free_and_set__next_list(list, new_node, rest);
}

char	*get_next_line(int fd)
{
	static t_list	*list;
	char			*line;
	int				len;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free_and_set__next_list(&list, NULL, NULL);
		return (NULL);
	}
	read_and_create_list(&list, fd);
	if (!list)
		return (NULL);
	len = line_length(&list);
	line = (char *)malloc(sizeof(char) * len + 1);
	if (!line)
		return (NULL);
	extract_line(list, line);
	if (!line)
	{
		free(line);
		free_and_set__next_list(&list, NULL, NULL);
		return (NULL);
	}
	clean_list(&list);
	return (line);
}
