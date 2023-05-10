/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:23:52 by djacobs           #+#    #+#             */
/*   Updated: 2023/04/25 18:23:54 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
*	this function frees everything allocated in case of an error
*	including closing the pipes and file descriptors 
*/
int	free_all(t_pipex _pipex, int mode)
{
	int	i;

	i = 0;
	if (mode == 0)
		return (close(_pipex.pipe_fd[1]), close(_pipex.pipe_fd[0]), 0);
	while (_pipex.paths[i])
		free(_pipex.paths[i++]);
	free(_pipex.paths[i]);
	free(_pipex.paths);
	i = 0;
	while (_pipex.args[i])
		free(_pipex.args[i++]);
	free(_pipex.args[i]);
	free(_pipex.args);
	close(_pipex.pipe_fd[0]);
	close(_pipex.pipe_fd[1]);
	if (mode == 2)
		return (close(_pipex.i), close(_pipex.o), 0);
	return (0);
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
	pathname = malloc(sizeof(char) * ((ft_strlen(path) + ft_strlen(name)) + 2));
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
	pathname[i] = '\0';
	return (pathname);
}

/*
*	check for file and command permission and if it exists
*/
int	check_file(t_pipex _pipex, char **argv, int i)
{
	if (!i)
	{
		if (access(argv[1], F_OK | R_OK) == -1)
			return (free_all(_pipex, 1), perror("error eldest proc"), 1);
		if (_pipex.args[0] == NULL)
			return (free_all(_pipex, 1), perror("error eldest proc"), 1);
	}
	else if (i)
	{
		if (access(argv[4], F_OK) == 0)
			if (access(argv[4], W_OK) == -1)
				return (free_all(_pipex, 1), perror("error youngest proc"), 1);
		if (_pipex.args[0] == NULL)
			return (free_all(_pipex, 1), perror("error youngest proc"), 1);
	}
	else if (i < 0)
		return (free_all(_pipex, 1), perror("exec error : cmd not found"), 1);
	return (0);
}

/*
*	(function breakdown)
*	112:	create a list of the paths from env $PATH
*	113:	create the arguments for execve and adding the infile
*	114:	check for permission and cmd paths
*	117:	replace the input file with stdin
*	120:	replace write-end of the pipe with stdout 
*	121:	close the pipe since stdout is write end of pipe 
*	123:	build the pathname with  the path and cmd
*	124>:	loop over every path given by env until the right path is found
*	129:	execute the command with the pathname, if it fails loop over the path
*/
void	eldest(char **argv, char **env, t_pipex _pipex)
{
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[2], ' ');
	if (check_file(_pipex, argv, 0))
		exit(1);
	_pipex.i = open(argv[1], O_RDONLY | __O_CLOEXEC);
	dup2(_pipex.i, STDIN_FILENO);
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/usr/bin:/usr/sbin:/bin:/sbin", ':');
	dup2(_pipex.pipe_fd[1], STDOUT_FILENO);
	close(_pipex.pipe_fd[1]);
	close(_pipex.pipe_fd[0]);
	_pipex.pathname = pathname(_pipex.paths[_pipex.num++], _pipex.args[0]);
	while (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
	{
		free(_pipex.pathname);
		_pipex.pathname = pathname(_pipex.paths[_pipex.num++], _pipex.args[0]);
		if (_pipex.pathname == NULL)
		{
			perror("Error eldest cmd not found");
			exit(EXIT_FAILURE);
		}
	}
	execve(_pipex.pathname, _pipex.args, env);
}

/*
*	the youngest child is the same as the eldest but the input
*	and output are different and i wait for the previous process
*/
void	youngest(char **argv, char **env, t_pipex _pipex)
{
	_pipex.stat_loc = NULL;
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[3], ' ');
	if (check_file(_pipex, argv, 1))
		exit(1);
	_pipex.o = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC | __O_CLOEXEC, 0644);
	dup2(_pipex.o, STDOUT_FILENO);
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/ust/bin:/usr/sbin:/bin:/sbin", ':');
	dup2(_pipex.pipe_fd[0], STDIN_FILENO);
	close(_pipex.pipe_fd[1]);
	close(_pipex.pipe_fd[0]);
	_pipex.pathname = pathname(_pipex.paths[_pipex.num++], _pipex.args[0]);
	while (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
	{
		free(_pipex.pathname);
		_pipex.pathname = pathname(_pipex.paths[_pipex.num++], _pipex.args[0]);
		if (_pipex.pathname == NULL)
		{
			perror("Error youngest cmd not found");
			exit(EXIT_FAILURE);
		}
	}
	execve(_pipex.pathname, _pipex.args, env);
}
