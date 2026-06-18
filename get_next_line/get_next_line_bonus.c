/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:35:20 by ycinarog          #+#    #+#             */
/*   Updated: 2025/08/07 16:11:27 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*copy_free(char *stash, char *remainder)
{
	free(stash);
	return (remainder);
}

char	*check(char *stash)
{
	if (!stash || !*stash)
		return (NULL);
	if (!ft_chr(stash, '\n'))
		return (ft_dup(stash));
	return (ft_sub(stash, 0, ft_chr(stash, '\n') - stash + 1));
}

char	*get_remainder(char *stash)
{
	char	*remainder;
	char	*newline_ptr;

	newline_ptr = ft_chr(stash, '\n');
	if (!newline_ptr)
		return (NULL);
	remainder = ft_sub(newline_ptr + 1, 0, ft_len(newline_ptr + 1));
	return (remainder);
}

char	*allocate_buffer(int fd)
{
	char	*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*buffer;
	char		*line;
	ssize_t		bytes;

	buffer = allocate_buffer(fd);
	if (!buffer)
		return (NULL);
	bytes = 1;
	while (!ft_chr(stash[fd], '\n') && bytes != 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buffer), free(stash[fd]), stash[fd] = NULL, NULL);
		buffer[bytes] = '\0';
		stash[fd] = ft_join(stash[fd], buffer);
	}
	free(buffer);
	line = check(stash[fd]);
	if (ft_chr(stash[fd], '\n'))
		stash[fd] = copy_free(stash[fd], get_remainder(stash[fd]));
	else
		stash[fd] = copy_free(stash[fd], NULL);
	return (line);
}
