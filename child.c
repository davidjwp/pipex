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

void	free_all(char **paths, char **args)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths[i]);
	free(paths);
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args[i]);
	free(args);
}
/*
*	add the cmd at the end of the path after adding '/'
*/
char	*pathname(char *path, char *name)
{
	char	*pathname;
	int		i;

	if (path == NULL)
		return (NULL);
	pathname = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(name) + 2));
	if (!pathname)
		exit(1);
	i = 0;
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

int	check_file(char **argv, int i)
{
	if (access(argv[1], F_OK) == -1)
		return (perror("error fork1"), 1);
	else if (access(argv[1], R_OK) == -1)
		return (perror("error fork1"), 1);
	return (0);
}
/*
*	(function breakdown)
*	101:		create a list of the paths from env $PATH
*	102:		create the arguments for execve and adding the infile
*	103:		replace write-end of the pipe with stdout 
*	104:		close read-end of the pipe
*	105:		concatenate the cmd name to path
*	106:		execute the command with the pathname (path + cmd), if it fails loop over the path  
*	109:		if there is no more paths try an absolute path
*	111>:	if even that fails send an error, free all and exit
*/
void	eldest(char **argv, char **env, t_pipex _pipex)//22 done 
{
	if (check_file(argv , 0))
		exit(1);
	_pipex.in = open(argv[1], O_RDONLY | O_CREAT);
	dup2(_pipex.in, STDIN_FILENO);
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[2], ' ');//this might be the cause for the valgrind error
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/usr/bin:/usr/sbin:/bin:/sbin", ':');
	dup2(_pipex.pipe_fd[1], STDOUT_FILENO);
	close(_pipex.pipe_fd[0]);
	_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);//use of uninitialized value 
	while (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
	{
		free(_pipex.pathname);
		_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);//use of uninitialized value 
	}
	if (_pipex.pathname == NULL)
	{
		free_all(_pipex.paths, _pipex.args);
		perror("execve error : wrong path/cmd");
		exit(1);
	}
	execve(_pipex.pathname, _pipex.args, NULL);
}

/*
*	(this function is pretty much the same as the eldest child 
*	though some things are different) 
*	:	check for file access if the file does not exist create it
*/
void	youngest(char **argv, char **env, t_pipex _pipex)//unfinished
{
	waitpid(_pipex.eldest_pid, _pipex.stat_loc, WUNTRACED);
	if (access(argv[4], F_OK | W_OK) == -1)
		_pipex.out = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY , 0644);
	dup2(_pipex.out, STDOUT_FILENO);//this redirect doesn't work
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[3], ' ');
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/ust/bin:/usr/sbin:/bin:/sbin", ':');
	dup2(_pipex.pipe_fd[0], STDIN_FILENO);
	close(_pipex.pipe_fd[1]);
	_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);
	while (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
	{
		free(_pipex.pathname);
		_pipex.pathname = pathname(_pipex.paths[_pipex.i++], _pipex.args[0]);
	}
	if (_pipex.pathname == NULL)
	{
		free_all(_pipex.paths, _pipex.args);
		perror("execve error : yongest child");
		exit (1);
	}
	execve(_pipex.pathname, _pipex.args, NULL);
}
