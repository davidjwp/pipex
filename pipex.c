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
*	:	check argc is right	
*	:	create a pipe
*	:	replace infile with stdin
*	:	fork the processes into eldest and then youngest processes
*/
int	main(int argc, char **argv, char **env)//21
{
	t_pipex	_pipex;

	if (argc != 5)
		return (0);
	if (pipe(_pipex.pipe_fd) == -1)
		return (perror("pipe error"), 0);
	_pipex.eldest_pid = fork();
	if (!_pipex.eldest_pid)
		eldest(argv, env, _pipex);
	else if (_pipex.eldest_pid == -1)
		return (perror("fork1 error"), 0);
	// while (read(_pipex.pipe_fd[0], _pipex.buf, BUFSIZ))
		// write (STDOUT_FILENO, _pipex.buf, ft_strlen(_pipex.buf));
	 _pipex.youngest_pid = fork();
	 if (!_pipex.youngest_pid)
		youngest(argv, env, _pipex);
	else if (_pipex.youngest_pid == -1)
		return (perror("fork2 error"), close(_pipex.in), close(_pipex.out), 0);
	return (0);
}
