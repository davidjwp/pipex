/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 09:03:51 by djacobs           #+#    #+#             */
/*   Updated: 2023/05/08 15:42:42 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

int	main()
{
    char    *line;
	char	*buffer;
    int		i;
	int		r;
	char	c;

	i = 0;
	r = 0;
    int	fd = open("file", O_RDONLY);
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	r = read(fd, &c, 1);
	while (r && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		r = read(fd, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	line = buffer;
	free(buffer);
	return (r);
}
