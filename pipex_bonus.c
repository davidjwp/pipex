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

int	open_file(char **argv, int argc, int in)
{
	int	fd;

	if (in)
	{
		fd = open(argv[1], O_RDONLY | __O_CLOEXEC);
		if (access(fd, F_OK | R_OK) == 0)
			return (fd);
		else
			return (perror("Error file not found :"), 0);
	}
	fd = open(argv[argc], O_CREAT | O_WRONLY | O_TRUNC | __O_CLOEXEC, 0644 );
	return (fd);
}

/*
*	(function breakdown)
*	64:	check argc is right	
*	66:	create a pipe
*	68:	fork the process into eldest
*	73:	fork the processes into youngest
*	78:	wait for the last child so as to not have an orphan process
*/
void	execute(char **argv, char **env, t_pipex _pipex)
{
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[3], ' ');
	if (check_file(_pipex, argv, 0))
		exit(1);
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/usr/bin:/usr/sbin:/bin:/sbin", ':');
	close(_pipex.pipe_fd[1]);
	close(_pipex.pipe_fd[0]);
	while (_pipex.paths[i] != NULL)
	{
		_pipex.pathname = pathname(_pipex.paths[i++], _pipex.args[0]);
		if (access(_pipex.pathname, X_OK) == 0 && _pipex.pathname != NULL)
			break;
		else if (_pipex.paths == NULL)
			return (perror(""), 0);
		free(_pipex.pathname);
	}
	execve(_pipex.pathname, _pipex.args, NULL);
}

int	child_process(char **argv, char **env, t_pipex _pipex, int index)
{
	int	pid;

	pid = fork();
	if (pipe(_pipex.pipe_fd) == -1)
			return (perror ("pipe error"), 0);
	else if (pid > 0)//fork opens stdout to pipe  
	{
		dup2(_pipex.pipe_fd[1], STDOUT_FILENO);
		close(_pipex.pipe_fd[0]);
		execute(argv[index + 2], env, _pipex);
	}
	else if (pid == 0)
	{
		close(_pipex.pipe_fd[1]);
		dup2(_pipex.pipe_fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
	return (0);
}

int main(int argc, char **argv, char **env)//23
{
	t_pipex	_pipex;
	int		status;
	int		i;

	i = 0;
	_pipex.cmd_num = (argc - 3);
	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)//go into here_doc 
			return (here_doc(argc, argv, env, _pipex));
		else//else init infile and outfile then replace stdin with the infile so that
		{//the first execution is done with infile as the input
			_pipex.i = open_file(argv, argc, 1);
			_pipex.o = open_file(argv, argc, 0);
			dup2(_pipex.i, STDIN_FILENO);
		}
		if (!_pipex.i || !_pipex.o)//if file fd error
			return (close (_pipex.i), close (_pipex.o), 0);
		while (i < _pipex.cmd_num)//will have to calibrate to exec the right amount of cmds 
			child_process(argv env, _pipex, i++);
		dup2(_pipex.o, STDOUT_FILENO);
		execute(argv[], env);
	}
	return (wait(NULL), free_all(_pipex, 0);, free_all(_pipex, 1), 0);
}
