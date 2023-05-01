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
*	(function breakdown)
*	27:	check argc is right	
*	29:	create a pipe
*	31:	fork the process into eldest
*	36:	fork the processes into youngest
*	42: wait for the last child so as to not have an orphan process
*/
int	main(int argc, char **argv, char **env)
{
	t_pipex	_pipex;
	int		status;

	if (argc != 5)
		return (0);
	if (pipe(_pipex.pipe_fd) == -1)
		return (perror("pipe error"), 0);
	_pipex.eldest_pid = fork();
	if (!_pipex.eldest_pid)
		eldest(argv, env, _pipex);
	else if (_pipex.eldest_pid == -1)
		return (perror("fork1 error"), 0);
	_pipex.youngest_pid = fork();
	if (!_pipex.youngest_pid)
		youngest(argv, env, _pipex);
	else if (_pipex.youngest_pid == -1)
		return (perror("fork2 error"), close(_pipex.in), close(_pipex.out), 0);
	wait(&status);
	close(_pipex.pipe_fd[0]);
	close(_pipex.pipe_fd[1]);
	return (0);
}
