/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:29:10 by cscache           #+#    #+#             */
/*   Updated: 2025/04/30 09:33:54 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*str;

	i = (int)ft_strlen(s);
	str = (char *)s;
	while (i >= 0)
	{
		if (str[i] == (unsigned char)c)
			return (str + i);
		i--;
	}
	return (NULL);
}
