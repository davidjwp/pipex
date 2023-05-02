/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 17:06:36 by djacobs           #+#    #+#             */
/*   Updated: 2023/04/01 17:06:38 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
*	this is kinda stupid because i'm doing this twice while it would 
*	be much smarter to integrate this into the child process and 
*	free all if the command is not found, it would make the code
*	faster and more readable but i'm lazy so fuck it lol 
*/
int	check_cmd(t_pipex _pipex)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (_pipex.paths[i] != NULL)
	{
		_pipex.pathname = pathname(_pipex.paths[i++], _pipex.args[0]);
		if (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
			free(_pipex.pathname);
		else
			return (0);
		if (_pipex.paths[i] == NULL && !n)
		{
			_pipex.paths = ft_split("/ust/bin:/usr/sbin:/bin:/sbin", ':');
			n++;
			i = 0;
		}
	}
	n = 0;
	while (_pipex.paths[n])
		free(_pipex.paths[n++]);
	free(_pipex.paths[n]);
	free(_pipex.paths);
	return (1);
}

/*
*	(function breakdown)
*	64:	check argc is right	
*	66:	create a pipe
*	68:	fork the process into eldest
*	73:	fork the processes into youngest
*	78:	wait for the last child so as to not have an orphan process
*/
int	main(int argc, char **argv, char **env)
{
	t_pipex	_pipex;
	int		status;

	_pipex.num = 0;
	if (argc != 5)
		return (0);
	if (pipe(_pipex.pipe_fd) == -1)
		return (perror("pipe error"), 0);
	_pipex.eldest_pid = fork();
	if (!_pipex.eldest_pid)
		eldest(argv, env, _pipex);
	else if (_pipex.eldest_pid == -1)
		return (free_all(_pipex, 1), perror("fork1 error"), 0);
	_pipex.youngest_pid = fork();
	if (!_pipex.youngest_pid)
		youngest(argv, env, _pipex);
	else if (_pipex.youngest_pid == -1)
		return (free_all(_pipex, 1), perror("fork2 error"), 0);
	wait(&status);
	free_all(_pipex, 0);
	return (0);
}
