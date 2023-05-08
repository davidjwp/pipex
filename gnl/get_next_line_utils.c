/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 19:30:47 by djacobs           #+#    #+#             */
/*   Updated: 2022/12/23 19:30:49 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*join;
	size_t	size;
	int		i;

	i = 0;
	if (!s1)
		s1 = ft_calloc(1, sizeof(char));
	if (!s2)
		return (free(s2), NULL);
	size = ft_strlen(s1) + ft_strlen(s2);
	join = (char *)ft_calloc(size + 1, sizeof(char));
	if (!join)
		return (free(s1), free(join), NULL);
	while (s1[i])
		*join++ = s1[i++];
	while (*s2)
		*join++ = *s2++;
	join -= size;
	return (free(s1), join);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*ptr;
	size_t	ptr_size;

	ptr_size = (int)(nmemb * size);
	if (size != 0 && nmemb != (ptr_size / size))
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	while (ptr_size--)
		ptr[ptr_size] = 0;
	return (ptr);
}

int	check_line(char *buf)
{
	int	i;

	i = 0;
	if (!buf || !*buf)
		return (0);
	while (buf[i])
	{
		if (buf[i] == '\n')
		{
			if (!i)
				return (1);
			return (i);
		}
		i++;
	}
	return (0);
}
