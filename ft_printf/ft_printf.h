/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:57:08 by ycinarog          #+#    #+#             */
/*   Updated: 2025/07/21 15:23:37 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>
# include <stdio.h>

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putunbr_fd(unsigned int n, int fd);
void	ft_putnbr_base_fd(unsigned long nbr, const char *base, int fd);
int		ft_hexlen(unsigned long nbr);
int		ft_numlen(int n);
int		ft_numulen(unsigned int n);
size_t	ft_strlen(const char *s);
int		format_printf_first(const char *format, va_list *args, int i);
int		format_printf_last(const char *format, va_list *args, int i);
int		ft_printf(const char *format, ...);

#endif