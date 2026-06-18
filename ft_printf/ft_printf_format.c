/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:33:00 by ycinarog          #+#    #+#             */
/*   Updated: 2025/07/21 15:35:27 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	format_printf_first(const char *format, va_list *args, int i)
{
	int		nums;
	char	*big_nums;

	nums = 0;
	if (format[i] == 'c')
	{
		nums = va_arg(*args, int);
		ft_putchar_fd(nums, 1);
		return (1);
	}
	if (format[i] == 's')
	{
		big_nums = va_arg(*args, char *);
		if (!big_nums)
			return (write(1, "(null)", 6));
		ft_putstr_fd(big_nums, 1);
		return (ft_strlen(big_nums));
	}
	if (format[i] == 'd' || format[i] == 'i')
	{
		nums = va_arg(*args, int);
		ft_putnbr_fd(nums, 1);
		return (ft_numlen((long)nums));
	}
	return (0);
}

int	format_printf_last(const char *format, va_list *args, int i)
{
	unsigned int	x;

	if (format[i] == 'u')
	{
		x = va_arg(*args, unsigned int);
		ft_putunbr_fd(x, 1);
		return (ft_numulen((unsigned long)x));
	}
	else if (format[i] == 'x')
	{
		x = va_arg(*args, unsigned int);
		ft_putnbr_base_fd(x, "0123456789abcdef", 1);
		return (ft_hexlen(x));
	}
	else if (format[i] == 'X')
	{
		x = va_arg(*args, unsigned int);
		ft_putnbr_base_fd(x, "0123456789ABCDEF", 1);
		return (ft_hexlen(x));
	}
	else if (format[i] == '%')
	{
		return (write(1, "%", 1));
	}
	return (0);
}
