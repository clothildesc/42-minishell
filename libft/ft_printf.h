/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:37:55 by cscache           #+#    #+#             */
/*   Updated: 2025/06/10 17:52:17 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

void	ft_putchar_pf(char c, int *count);
void	ft_putstr_pf(char *str, int *count);
void	ft_putint_base_pf(int n, char *base, int *count);
void	ft_putun_base_pf(unsigned int n, char *base, int *count);
void	ft_putul_base_pf(unsigned long n, char *base, int *count);
int		ft_printf(const char *format, ...);

#endif