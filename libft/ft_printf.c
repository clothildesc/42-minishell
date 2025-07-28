/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:25:49 by cscache           #+#    #+#             */
/*   Updated: 2025/06/10 17:17:26 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_handle_ptr(va_list args, int *count)
{
	void	*ptr;

	ptr = va_arg(args, void *);
	if (!ptr)
		ft_putstr_pf("(nil)", count);
	else
	{
		ft_putstr_pf("0x", count);
		ft_putul_base_pf((unsigned long)ptr, "0123456789abcdef", count);
	}
}

static void	ft_format(char c, va_list args, int *count)
{
	if (c == '%')
		ft_putchar_pf('%', count);
	else if (c == 'c')
		ft_putchar_pf(va_arg(args, int), count);
	else if (c == 'd' || c == 'i')
		ft_putint_base_pf(va_arg(args, int), "0123456789", count);
	else if (c == 's')
		ft_putstr_pf(va_arg(args, char *), count);
	else if (c == 'u')
		ft_putun_base_pf(va_arg(args, unsigned int), "0123456789", count);
	else if (c == 'x')
		ft_putun_base_pf(va_arg(args, unsigned int), "0123456789abcdef", count);
	else if (c == 'X')
		ft_putun_base_pf(va_arg(args, unsigned int), "0123456789ABCDEF", count);
	else if (c == 'p')
		ft_handle_ptr(args, count);
	else
		ft_putchar_pf('%', count);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	va_start(args, format);
	i = 0;
	count = 0;
	if (!format)
		return (-1);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			ft_format(format[i], args, &count);
		}
		else
			ft_putchar_pf(format[i], &count);
		if (count != -1)
			i++;
		else
			break ;
	}
	va_end(args);
	return (count);
}
