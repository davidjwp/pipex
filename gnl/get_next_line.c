/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 10:10:23 by djacobs           #+#    #+#             */
/*   Updated: 2022/12/23 10:10:25 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_buf_line( int fd, char *buf)
{
	char	*line;
	int		bytes_read;

	line = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!line)
		return (NULL);
	bytes_read = 1;
	while (!check_line(buf) && bytes_read)
	{
		bytes_read = read(fd, line, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buf), free(line), NULL);
		line[bytes_read] = 0;
		buf = ft_strjoin(buf, line);
	}
	return (free(line), buf);
}

static char	*give_line(char *buf)
{
	char	*line;
	int		i;

	i = 0;
	if (!*buf)
		return (NULL);
	while (buf[i] && buf[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buf[i] != '\n' && buf[i])
	{
		line[i] = buf[i];
		i++;
	}
	if (buf[i] == '\n')
		line[i] = '\n';
	if (!*buf)
		return (free(line), NULL);
	return (line);
}

static char	*clear_buf(char *buf)
{
	char	*clear;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (!buf[i])
		return (free(buf), NULL);
	i++;
	clear = malloc(sizeof(char) * (ft_strlen(buf) - i + 1));
	if (!clear)
		return (NULL);
	while (buf[i] != '\0')
		clear[j++] = buf[i++];
	clear[j] = 0;
	return (free(buf), clear);
}

char	*get_next_line(int fd)
{
	static char	*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = read_buf_line(fd, buf);
	if (!buf)
		return (NULL);
	line = give_line(buf);
	buf = clear_buf(buf);
	return (line);
}
