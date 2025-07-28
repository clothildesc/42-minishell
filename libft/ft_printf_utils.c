/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:20:42 by cscache           #+#    #+#             */
/*   Updated: 2025/06/10 17:16:14 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar_pf(char c, int *count)
{
	if (write(1, &c, 1) != -1)
		*count += 1;
	else
		*count = -1;
}

void	ft_putstr_pf(char *str, int *count)
{
	int	len;

	len = 0;
	if (!str)
		return (ft_putstr_pf("(null)", count));
	while (str[len])
		len++;
	if (write(1, str, len) != -1)
		*count += len;
	else
		*count = -1;
}

void	ft_putint_base_pf(int n, char *base, int *count)
{
	long	nb;
	int		base_len;

	nb = n;
	base_len = 0;
	if (*count == -1)
		return ;
	while (base[base_len])
		base_len++;
	if (nb < 0)
	{
		ft_putchar_pf('-', count);
		if (*count == -1)
			return ;
		nb = -nb;
	}
	if (nb >= base_len)
		ft_putint_base_pf(nb / base_len, base, count);
	ft_putchar_pf(base[nb % base_len], count);
}

void	ft_putun_base_pf(unsigned int n, char *base, int *count)
{
	unsigned int	base_len;

	base_len = 0;
	if (*count == -1)
		return ;
	while (base[base_len])
		base_len++;
	if (n >= base_len)
		ft_putun_base_pf(n / base_len, base, count);
	ft_putchar_pf(base[n % base_len], count);
}

void	ft_putul_base_pf(unsigned long n, char *base, int *count)
{
	unsigned long	base_len;

	base_len = 0;
	if (*count == -1)
		return ;
	while (base[base_len])
		base_len++;
	if (n >= base_len)
		ft_putul_base_pf(n / base_len, base, count);
	ft_putchar_pf(base[n % base_len], count);
}
