/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clothildescache <clothildescache@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 22:27:17 by clothildesc       #+#    #+#             */
/*   Updated: 2025/04/28 22:27:19 by clothildesc      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t		src_len;
	size_t		dst_len;
	size_t		i;
	const char	*dest;

	dest = (const char *)dst;
	src_len = ft_strlen(src);
	dst_len = ft_strlen(dest);
	if (siz <= dst_len)
		return (src_len + siz);
	i = 0;
	while (src[i] && (dst_len + i) < (siz - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
