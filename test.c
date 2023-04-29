#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

char	*envstr(char **envector, const char *envariable)//22
{
	int	i;
	int	i2;
	int	n;

	n = 0;
	i = 0;
	i2 = 0;
	while (envariable[n])
		n++;
	while (i < 60)
	{
		while (envector[i][i2] == envariable[i2])
			i2++;
		if (i2 == n)
		{
			while (n--)
				envector[i]++;
			return (++envector[i]);
		}
		i2 = 0;
		i++;
	}
	return (NULL);
}

char	*pathname(char *path, char *name)
{
	char	*pathname;
	int		i;

	if (path == NULL)
		return (NULL);
	pathname = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(name)) + 1);
	i = 0;
	while (path[i])
	{
		pathname[i] = path[i];
		i++;
	}
	pathname[i++] = '/';
	while (*name)
		pathname[i++] = *name++;
	return (pathname);
}

int	main(int argc, char **argv, char **env)
{
	// char	*path = envstr(env, "PATH");
	// if (!path)//absolute path if none found in PATH, 
	// 	path = "/bin/";

	// int	pipe_fd[2];
	// pipe(pipe_fd);
	// write (pipe_fd[1], "hello world", 11);
	// char	buffer[BUF_SIZ];
	// char	buffer2[BUF_SIZ];
	// read (pipe_fd[0], buffer, BUF_SIZ);
	// write (pipe_fd[1], "hello world", 11);
	// read (pipe_fd[0], buffer2, BUF_SIZ);
	
	// printf("%s\n", path);
	int	pipe_fd[2];
	int	pid;
	char	buf[BUFSIZ];

	pipe(pipe_fd);
	pid = fork();
	if (pid == 0)
	{
		char	**args = ft_split(argv[1], ' ');

		dup2 (pipe_fd[1], STDOUT);
		execve(pathname("/bin/", argv[1]), args, NULL);
	}
	return 0; 
}

/*
*	the envp in execve is a string of strings much like env[] in the main
*	program, if NULL is given it will inerit the current process env
/
*	args is like an internal simulation of argv where args[0] is like argv[0]
*	except that you can should whatever you want as arguments whereas argv[0] will 
*	always be the binary or executable so pathname in execve is like argv[0]
*
*	test-cases
*	/if a command that does not exist is given
*		.since command lines are pretty short, you can tell there is an error if it too long
*		.you can use access's flag X_OK to check if the command is executable  
*		.or just conditional with execve would probably be faster
*	/check for command flags
*		.you will have to take all the flags and dinamically add them to argv in execve so some parsing is needed
*	/if not enough arguments are given
*		.this exercise isn't really to replicate exactly how a pipe works, since here you have specific case with files
*	/if there is no file or only one file is missing
*		.even if the first file is not given, you should still execute the second and let the first command
*		.handle any error with the first file, meaning to get closer to an authentic implementation of a pipe
*		.you don't actually have to open the first file 
*	/you should also check if you have access to those files like the right permissions
*		.use access() to do that 
*	/no memory leaks
*	/if the pipe buffer overflows
*		.the way the shell works is, if you fill up the pipe buffer the write call will stop writing to the pipe buffer until 
*		.there is space, meaning that it technically can't overflow and you get what you read 
*		.BUT as long as there is unread data it will continue to feed it to the second command until all the data has been read
*		.so first it executes the first command then feeds that into the second, if there is more output from the first command 
*		.it will continue to execute the first command and feed i into the second until there is nothing to feed
*		.int	bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE); this should let you see how many bytes have been read in the pipe buffer
*	/if fd NULL
*		.just protect every open
*	/make sure to close the file descriptors
*	/check for command access
*	/make sure to dup the fd's right, DO NOT REPLACE STDOUT/STDIN it's always the other way
*	/if there is no environment 
*/

/*
*	Here are the rules for the project(some of these exist because the subject does not make reference of them)
*		.note that this is for the mandatory part	
*
*	/1. the programm must have 5 arguments
*		.if there is more or less the program returns an error
*	/2.	every error case must be handled
*		.0 if there is not enough arguments
*		.1 if the wrong command is entered
*		.2 some errors are handled by the commands themselves but not by the code, these are:
*			-0 if the input file does not exist
*			-1 if you do not have the right permission for the file
*/