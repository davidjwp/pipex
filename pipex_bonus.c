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
	if (_pipex.pathname != NULL && !fds)
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
	if (fds)
	{
		close(_pipex.pipe_fd[0]);
		close(_pipex.pipe_fd[1]);
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
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC | __O_CLOEXEC, 0644 );
	if (access(file, F_OK | W_OK) == 0)
		return (fd);
	else
		return (perror("Error outfile :"), 0);
}

/*
	give gnl the address of the pointer but in gnl the argument has to be 
	a double pointer since you dereference the address of a pointer type
	thus initializing the argument with a pointer type, the only way you 
	can initialize this variable is with a double pointer 
*/
int	here_doc(t_pipex _pipex, char *lmt)//21
{
	pid_t	pid;
	char	*line;
	int		lmt_l;

	lmt_l = ft_strlen(lmt);
	line = NULL;
	if (pipe(_pipex.pipe_fd) == -1)
		return (perror("Error pipe"), 0);
	pid = fork();
	if (pid == -1)
		return (perror("Error fork"), 0);
	if (pid == 0)
	{
		close(_pipex.pipe_fd[0]);
		write (1, "heredoc> ", 9);
		while (gnl(&line))
		{
			if (ft_strncmp(line, lmt, lmt_l) == 0 && line[lmt_l] == '\n')
				exit(EXIT_SUCCESS);
			write (_pipex.pipe_fd[1], line, ft_strlen(line));
			write (1, "heredoc> ", 9);
		}
	}
	return (close(_pipex.pipe_fd[0]), close(_pipex.pipe_fd[1]), 0);
}

int main(int argc, char **argv, char **env)
{
	t_pipex	_pipex;

	if (argc < 5)
		return (0);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		_pipex.index = 3;
		_pipex.o = open_file(argv[argc - 1], 0);
		here_doc(_pipex, argv[2]);
	}
	else
	{
		_pipex.index = 2;
		_pipex.i = open_file(argv[1], 1);
		_pipex.o = open_file(argv[argc - 1], 0);
		dup2(_pipex.i, STDIN_FILENO);
	}
	if (!_pipex.i || !_pipex.o)
		return (close (_pipex.i), close (_pipex.o), perror("Error :"),0);
	while (_pipex.index < (argc - 2)) 
		child_process(argv[_pipex.index++], env, _pipex);
	dup2(_pipex.o, STDOUT_FILENO);
	execute(argv[_pipex.index], env, _pipex);
	return (wait(NULL), free_all(_pipex, 1), 0);
}
