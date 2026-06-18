/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:35:17 by ycinarog          #+#    #+#             */
/*   Updated: 2025/08/07 18:04:44 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 27
# endif

# include <stdlib.h>
# include <unistd.h>

char	*ft_join(char *s1, char *s2);
char	*ft_chr(char *s, int c);
char	*ft_dup(const char *s1);
char	*ft_sub(char *s, unsigned int start, size_t len);
size_t	ft_len(const char *s);
char	*get_next_line(int fd);
char	*copy_free(char *stash, char *remainder);
char	*check(char *stash);
char	*allocate_buffer(int fd);
char	*get_remainder(char *stash);
char	*allocate_buffer(int fd);

#endif
