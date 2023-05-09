/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:14:03 by djacobs           #+#    #+#             */
/*   Updated: 2023/05/03 13:14:04 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/*
	the gnl function will read standard input into buf and give the result
	to line acting like a dangling pointer, buf is allocated memory given the 
	stdin until a new line (meaning if you were to write in the here_doc when
	you push enter) then give the address of the buf pointer to line, you might
	think "why not allocate to line directly dumbass", well it's because i
	have to save lines thanks to this bitch of a norm and this way when i
	return i get to have line containing the string so that i can 
	use it and free it
*/
int	gnl(char **line)
{
	char	*buf;
	int		n;
	char	c;
	int		i;

	i = 0;
	write (1, "heredoc> ", 9);
	buf = malloc(sizeof(char) * BUFSIZ);
	n = read(STDIN_FILENO, &c, 1);
	while (n && c != '\n' && c != '\0')
	{
		if (c != 'n' && c != '\0')
			buf[i] = c;
		n = read(STDIN_FILENO, &c, 1);
		i++;
	}
	buf[i] = '\n';
	buf[++i] = 0;
	*line = buf;
	return (n);
}

/*
*	this function simply creates the pathname needed by execve
*	it'll add the command given to the end of the path after adding '/'
*/
char	*pathname(char *path, char *name)
{
	char	*pathname;
	int		i;

	i = 0;
	if (path == NULL)
		return (NULL);
	pathname = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(name) + 2));
	if (!pathname)
		return (perror("pathname error"), free(pathname), "error");
	while (path[i])
	{
		pathname[i] = path[i];
		i++;
	}
	pathname[i++] = '/';
	while (*name)
		pathname[i++] = *name++;
	pathname[i] = 0;
	return (pathname);
}

/*	
	execute is exactly what it sounds like, it executes, one thing i do is 
	cicle through all the environment paths to find the right pathname for 
	that command, if nothing then the command either does not allow permissions
	or straight up doesn't exist then i free everything, close all the file
	descriptors and exit the process, else executing the command, 
	also if no env is given i will use an absolute path anyway which should
	garantee to find the command 
*/
void	execute(char *argv, char **env, t_pipex _pipex)
{
	_pipex._int = 0;
	if (!_pipex.o)
	{
		close(_pipex.i);
		exit(EXIT_FAILURE);
	}
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv, ' ');
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/usr/bin:/usr/sbin:/bin:/sbin", ':');
	while (_pipex.paths[_pipex._int] != NULL)
	{
		_pipex.pathname = pathname(_pipex.paths[_pipex._int++], _pipex.args[0]);
		if (access(_pipex.pathname, X_OK) == 0 && _pipex.pathname != NULL)
			break ;
		else if (_pipex.paths == NULL)
		{
			free_all(_pipex, 0);
			perror("Error path not found :");
			exit(EXIT_FAILURE);
		}
		free(_pipex.pathname);
	}
	execve(_pipex.pathname, _pipex.args, env);
}

/*
	this function creates the pipe and forks into the child process, it'll
	check in the first command if the input file is good and exit the child
	process if the infile doesn't exist or the permissions aren't right, this
	let's all the other commands work no problem.

	in the child process i replace stdout with my write end of the pipe so that
	the output is redirected to the pipe then close all the other file
	descriptors and execute, in the parent process i close the write end of 
	the pipe and replace the read end of the pipe with stdin and close the pipe
	completely, so in the next child process the stdin will be whatever was
	written to the pipe.

	btw a cool thing i learned is that closing the pipe completely doesn't
	mean that the std fd will be closed since i replaced it, it's like
	passing a memory address.
*/
int	child_process(char *argv, char **env, t_pipex _pipex)
{
	pid_t	pid;

	if (pipe(_pipex.pipe_fd) == -1)
		return (perror ("pipe error"), 0);
	pid = fork();
	if (pid == -1)
		return (perror("fork error :"), 0);
	if (pid == 0)
	{
		dup2(_pipex.pipe_fd[1], STDOUT_FILENO);
		free_all(_pipex, 1);
		if (_pipex._int == 3 && access(_pipex.input_file, F_OK | R_OK) == -1)
			exit(EXIT_FAILURE);
		execute(argv, env, _pipex);
	}
	else
	{
		close(_pipex.pipe_fd[1]);
		dup2(_pipex.pipe_fd[0], STDIN_FILENO);
		close (_pipex.pipe_fd[0]);
	}
	return (0);
}
