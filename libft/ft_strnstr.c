/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:52:02 by cscache           #+#    #+#             */
/*   Updated: 2025/04/29 16:43:47 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*bigstr;
	char	*littlestr;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	bigstr = (char *)big;
	littlestr = (char *)little;
	if (littlestr[0] == '\0')
		return (bigstr);
	while (bigstr[i] && i < len)
	{
		j = 0;
		while (littlestr[j] == bigstr[i + j] && (i + j < len))
		{
			if (littlestr[j + 1] == '\0')
				return (bigstr + i);
			j++;
		}
		i++;
	}
	return (NULL);
}
