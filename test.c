/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 09:03:51 by djacobs           #+#    #+#             */
/*   Updated: 2023/05/03 09:03:53 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

int	main(int argc, char **argv, char **env)
{
	int	pipe_num;
	int	_pipe[2];
	int	status;
	int	pid;
	int	i;


	i = 0;
	// pipe_num = (argc - 3) - 1; use this to determine the number of pipes 
	//not to be used in this test
	pipe(_pipe);
	pid = fork();//first fork
	if (!pid)
	{
		char	*pathname = "/bin/cat";
		char	*args[] = {"cat", NULL};

		int file = open("file", O_RDONLY | __O_CLOEXEC);
		dup2(file, STDIN_FILENO);
		dup2(_pipe[1], STDOUT_FILENO);
		close(_pipe[0]);
		close(_pipe[1]);
		execve(pathname, args, NULL);
	}
	while (/*i < pipe_num*/i < 4)
	{
		int mid_pipe[2];
		int	status;

		pipe(mid_pipe);
		if (!i)
			dup2(_pipe[0], mid_pipe[0]); 
		/*
		*	read of middle pipe becomes pipe read so that when the middle fork
		*	reads the last mid_pipe at first it'll read from the pipe at the
		*	beginning
		*/
		pid = fork();
		if (!pid)
		{
			char *pathname = "/bin/wc";
			char *args[] = {"wc", NULL};

			dup2(mid_pipe[0], STDIN_FILENO);//read from middle pipe
			if (i < (pipe_num - 1))
				dup2(mid_pipe[1], STDOUT_FILENO);//and write into the middle pipe
			else
				dup2(_pipe[1], STDOUT_FILENO);
			close(mid_pipe[0]);
			close(mid_pipe[1]);
			close(_pipe[0]);
			close(_pipe[1]);
			if (execve(pathname, args, NULL) == -1)
				perror("execve error : ");
		}
		i++;
	}
	char	buf[BUFSIZ];
	read(_pipe[0], buf, BUFSIZ);
	// pid = fork();//last fork
	// if (!pid)
	// {
	// 	char	*pathname = "/bin/wc";
	// 	char	*args[] = {"wc", NULL};

	// 	int	file = open("new_file", O_WRONLY | O_TRUNC | O_CREAT | __O_CLOEXEC, 0644);
	// 	dup2(_pipe[0], STDIN_FILENO);
	// 	dup2(file, STDOUT_FILENO);
	// 	close(_pipe[0]);
	// 	close(_pipe[1]);
	// 	execve(pathname, args, NULL);
	// }
	// wait(&status);
	// close(_pipe[0]);
	// close(_pipe[1]);
	return (0);
}
/*
*	this arragement works fine for multiple and will simulate how real pipes act
*	i just need to fix the redirection as it seems to be causing some issues 
*
*/

/*
*	the here_docs serves to feed an input block into a cmd ending it with 
*	a keyword being used as an end of file
*
*	it works like this cmd << EOF
*	in pipex it will work like this ./pipex here_doc EOF cmd
*	the here_doc serves as the first file input
*
*
*
*
*/
