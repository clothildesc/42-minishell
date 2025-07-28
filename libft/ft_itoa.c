/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:20:42 by cscache           #+#    #+#             */
/*   Updated: 2025/05/01 14:36:04 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_total_len(int n)
{
	int	temp;
	int	len;
	int	total;

	temp = n;
	len = 0;
	total = 0;
	while (temp != 0)
	{
		temp /= 10;
		len++;
	}
	if (n <= 0)
		total = len + 1 ;
	else
		total = len;
	return (total);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	int		len;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	len = ft_total_len(n);
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	ptr[len] = '\0';
	if (n < 0)
	{
		ptr[0] = '-';
		n = -n;
	}
	if (n == 0)
		ptr[0] = '0';
	while (n > 0)
	{
		ptr[len - 1] = (n % 10) + 48;
		n /= 10;
		len--;
	}
	return (ptr);
}
