/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:35:17 by cscache           #+#    #+#             */
/*   Updated: 2025/05/05 12:21:16 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	unsigned int	len_s;
	unsigned int	len_sub;
	char			*new_str;

	i = 0;
	len_s = ft_strlen(s);
	if (start >= len_s)
		len_sub = 0;
	else if ((unsigned int)len > len_s - start)
		len_sub = len_s - start;
	else
		len_sub = (unsigned int)len;
	new_str = malloc(sizeof(char) * (len_sub + 1));
	if (!new_str)
		return (NULL);
	while (i < len_sub && s[start + i])
	{
		new_str[i] = s[start + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
