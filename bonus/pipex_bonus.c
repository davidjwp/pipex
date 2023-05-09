/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:11:44 by djacobs           #+#    #+#             */
/*   Updated: 2023/05/03 13:11:45 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	if (s1 == NULL || s2 == NULL)
		return (1);
	while (*s1 == *s2 && n - 1)
	{
		s1++;
		s2++;
		n--;
	}
	if ((*s1 - *s2) == -128)
		return (1);
	return (*s1 - *s2);
}

void	free_all(t_pipex _pipex, int fds)
{
	int	i;

	i = 0;
	if (!fds)
	{
		while (_pipex.paths[i])
			free(_pipex.paths[i++]);
		free(_pipex.paths[i]);
		free(_pipex.paths);
		i = 0;
		while (_pipex.args[i])
			free(_pipex.args[i++]);
		free(_pipex.args[i]);
		free(_pipex.args);
		free(_pipex.pathname);
	}
	else
	{
		close(_pipex.pipe_fd[0]);
		close(_pipex.pipe_fd[1]);
		if (_pipex.i)
			close(_pipex.i);
		close(_pipex.o);
	}
}

int	open_file(char *file, int in)
{
	int	fd;

	if (in)
	{
		fd = open(file, O_RDONLY | __O_CLOEXEC);
		if (access(file, F_OK | R_OK) == 0)
			return (fd);
		else
			return (perror("Error infile :"), 0);
	}
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC | __O_CLOEXEC, 0644);
	if (access(file, F_OK | W_OK) == 0)
		return (fd);
	else
		return (perror("Error outfile :"), 0);
}

/*
	the here_doc function forks the parent process, the reason i fork here is
	because i must write all the input into a pipe since i'm trying to transfer
	data from one process to another, the way i do this is by closig the read
	end of the pipe i just created since i'm only writting into it then loop
	the gnl function which will read stdin for the input given and put it into
	line, then look if line is the same as lmt (short for limiter id) and i
	make sure to check that the line isn't the limiter plus a bunch of garbage
	like : here_doc STOP STOPapv321 this kindof line does not pass a regular
	here_doc, if the limiter is not found write the line into the pipe and free
	the line, else if it is found then exit the process, the parent process 
	is already waiting for the child, once it is there replace stdin with the
	read end of the pipe and close everything
*/
int	here_doc(t_pipex _pipex, char *lmt)
{
	char	*line;
	int		lmt_l;

	lmt_l = ft_strlen(lmt);
	line = NULL;
	if (pipe(_pipex.pipe_fd) == -1)
		return (perror("Error pipe"), 0);
	_pipex.pid = fork();
	if (_pipex.pid == 0)
	{
		close(_pipex.pipe_fd[0]);
		while (gnl(&line))
		{
			if (ft_strncmp(line, lmt, lmt_l) == 0 && line[lmt_l] == '\n')
				return (free(line), close(_pipex.pipe_fd[1]), close(_pipex.o),
					exit(EXIT_SUCCESS), 0);
			write (_pipex.pipe_fd[1], line, ft_strlen(line));
			free(line);
		}
	}
	else
		return (close(_pipex.pipe_fd[1]), dup2(_pipex.pipe_fd[0], STDIN_FILENO),
			wait(NULL), close(_pipex.pipe_fd[0]), 0);
	return (0);
}

/*
	first i check for the right number of arguments, then if the first argument
	is "here_doc" then go to here_doc else open the input and output file, if
	the input file is good then replace stdin with the input file, then 
	simply loop over the child processes until all the commands are done then
	do the last command while redirecting the output to the outfile
*/
int	main(int argc, char **argv, char **env)
{
	t_pipex	_pipex;

	if (argc < 5)
		return (0);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		_pipex._int = 3;
		_pipex.o = open_file(argv[argc - 1], 0);
		here_doc(_pipex, argv[2]);
	}
	else
	{
		_pipex._int = 2;
		_pipex.i = open_file(argv[1], 1);
		_pipex.o = open_file(argv[argc - 1], 0);
		if (_pipex.i)
			dup2(_pipex.i, STDIN_FILENO);
		_pipex.input_file = argv[1];
	}
	while (_pipex._int < (argc - 2))
		child_process(argv[_pipex._int++], env, _pipex);
	if (_pipex.o)
		dup2(_pipex.o, STDOUT_FILENO);
	execute(argv[_pipex._int], env, _pipex);
	return (wait(NULL), free_all(_pipex, 1), 0);
}
