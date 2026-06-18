/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:47:56 by ycinarog          #+#    #+#             */
/*   Updated: 2025/07/21 15:06:54 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	format_print(const char *format, va_list *args, int i)
{
	void			*ptr;
	unsigned long	addr;

	if (format[i] == 'c' || format[i] == 'd' || format[i] == 's'
		|| format[i] == 'i')
		return (format_printf_first(format, args, i));
	if (format[i] == 'p')
	{
		ptr = va_arg(*args, void *);
		if (!ptr)
			return (write(1, "(nil)", 5));
		addr = (unsigned long)ptr;
		ft_putstr_fd("0x", 1);
		ft_putnbr_base_fd(addr, "0123456789abcdef", 1);
		return (ft_hexlen(addr) + 2);
	}
	if (format[i] == 'u' || format[i] == 'x' || format[i] == 'X'
		|| format[i] == '%')
		return (format_printf_last(format, args, i));
	return (-1);
}

int	ft_printf(const char *format, ...)
{
	int		result;
	int		i;
	va_list	args;

	va_start(args, format);
	i = 0;
	result = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			result += format_print(format, &args, i + 1);
			i = i + 2;
		}
		else if (format[i] == '%')
			return (-1);
		else if (format[i])
		{
			ft_putchar_fd(format[i++], 1);
			result += 1;
		}
	}
	va_end(args);
	return (result);
}
