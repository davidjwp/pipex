// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   test.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/05/03 09:03:51 by djacobs           #+#    #+#             */
// /*   Updated: 2023/05/08 15:42:42 by djacobs          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */


// # include <unistd.h>
// # include <sys/wait.h>
// # include <fcntl.h>
// # include <stdio.h>
// # include <stdlib.h>

// int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	if (!n)
// 		return (0);
// 	while (*s1 == *s2 && n - 1)
// 	{
// 		s1++;
// 		s2++;
// 		n--;
// 	}
// 	if ((*s1 - *s2) == -128)
// 		return (1);
// 	return (*s1 - *s2);
// }

// int	ft_strlen(char *s)
// {
// 	int	i = 0;

// 	while (s[i])
// 		i++;
// 	return i;
// }
// int	gnl(char **line)
// {
// 	char	*buffer;
//     int		i;
// 	int		r;
// 	char	c;

// 	i = 0;
// 	r = 0;
//     int	fd = open("file", O_RDONLY);
// 	buffer = (char *)malloc(10000);
// 	if (!buffer)
// 		return (-1);
// 	r = read(fd, &c, 1);
// 	while (r && c != '\n' && c != '\0')
// 	{
// 		if (c != '\n' && c != '\0')
// 			buffer[i] = c;
// 		i++;
// 		r = read(fd, &c, 1);
// 	}
// 	buffer[i] = '\n';
// 	buffer[++i] = '\0';
// 	*line = buffer;
// 	free(buffer);
// 	return (r);
// }

// int main()
// {
//     char *line;

//     while(gnl(&line))
//     {
//         if (strncmp(line, "STOP") == 0)
//             return (printf("FOUND"),0);
//     }

//     return 0;
// }
// /*
// *	this arragement works fine for multiple and will simulate how real pipes act
// *	i just need to fix the redirection as it seems to be causing some issues 
// *
// */

// /*
// *	the here_docs serves to feed an input block into a cmd ending it with 
// *	a keyword being used as an end of file
// *
// *	it works like this cmd << EOF
// *	in pipex it will work like this ./pipex here_doc EOF cmd
// *	the here_doc serves as the first file input
// *
// *
// *
// *
// */

int main(int argc, char **argv)
{
    read();
}
