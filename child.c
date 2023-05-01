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

void	free_all(t_pipex _pipex)
{
	int	i;

	i = 0;
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
}

/*
*	truncate the command line arg to the path 
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
*	check for file permission and if it exists
*/
int	check_file(t_pipex _pipex, char **argv, int i)
{
	if (!i)
	{
		if (access(argv[1], F_OK | R_OK) == -1)
			{
				close(_pipex.pipe_fd[0]);
				close(_pipex.pipe_fd[1]);
				return (perror("error eldest process"), 1);
			}
	}
	else if (i)
	{
		if (access(argv[4], F_OK | W_OK) == -1)
		{
			close(_pipex.pipe_fd[0]);
			close(_pipex.pipe_fd[1]);
			return (perror("error youngest process"), 1);
		}
	}
	else if (i < 0)
	{
		free_all(_pipex);
		perror("execve error : command not found");
		exit (1);
	}
	return (0);
}

/*
*	(function breakdown)
*	91:		check for permission
*	94:		replace the input file with stdin
*	95:		create a list of the paths from env $PATH
*	96:		create the arguments for execve and adding the infile
*	99:		replace write-end of the pipe with stdout 
*	100:	close read-end of the pipe
*	101:	concatenate the cmd name to path
*	102>:	loop over every path given by env until the right path is found
*	107>:	if there no path works free everything exit 
*	113:	execute the command with the pathname, if it fails loop over the path
*/
void	eldest(char **argv, char **env, t_pipex _pipex)
{
	_pipex.i = 0;
	if (check_file(_pipex, argv, 0))
		exit(1);
	_pipex.in = open(argv[1], O_RDONLY | __O_CLOEXEC);
	dup2(_pipex.in, STDIN_FILENO);
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[2], ' ');
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/usr/bin:/usr/sbin:/bin:/sbin", ':');
	dup2(_pipex.pipe_fd[1], STDOUT_FILENO);
	close(_pipex.pipe_fd[0]);
	close(_pipex.pipe_fd[1]);
		printf("error close");
	_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);
	while (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
	{
		free(_pipex.pathname);
		_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);
	}
	if (_pipex.pathname == NULL)
		check_file(_pipex, argv, -1);
	_pipex.pipe_fd[1] |= FD_CLOEXEC;
	execve(_pipex.pathname, _pipex.args, NULL);
}

/*
*	the youngest child is the same as the eldest but the input
*	and output are different and i wait for the previous process
*/
void	youngest(char **argv, char **env, t_pipex _pipex)//does not create a file when "echo hello"
{
	_pipex.i = 0;
	_pipex.stat_loc = NULL;
	waitpid(_pipex.eldest_pid, _pipex.stat_loc, WUNTRACED);
	if (check_file(_pipex, argv, 1))
		exit(1);
	_pipex.out = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 644);
	dup2(_pipex.out, STDOUT_FILENO);
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[3], ' ');
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/ust/bin:/usr/sbin:/bin:/sbin", ':');
	dup2(_pipex.pipe_fd[0], STDIN_FILENO);
	close(_pipex.pipe_fd[1]);
	close(_pipex.pipe_fd[0]);
	close(5);//this is the last fd opened, idk what it is 
	_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);
	while (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
	{
		free(_pipex.pathname);
		_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);
	}
	if (_pipex.pathname == NULL)
		check_file(_pipex, argv, -1);
	_pipex.pipe_fd[0] |= FD_CLOEXEC;
	_pipex.out |= FD_CLOEXEC;
	execve(_pipex.pathname, _pipex.args, NULL);
}
