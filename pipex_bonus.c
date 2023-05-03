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

void	middle_child(char **argv, char **env, t_pipex _pipex, int mid_pipe)
{
	// _pipex.stat_loc = NULL;
	// waitpid(_pipex.eldest_pid, _pipex.stat_loc, WUNTRACED);
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv[3], ' ');
	if (check_file(_pipex, argv, 1))
		exit(1);
	// _pipex.o = open(argv[4], O_WRONLY | __O_CLOEXEC);
	dup2(mid_pipe[1], STDOUT_FILENO);
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/ust/bin:/usr/sbin:/bin:/sbin", ':');
	dup2(mid_pipe[1], STDIN_FILENO);
	close(mid_pipe[0]);
	close(mid_pipe[1]);
	_pipex.pathname = pathname(_pipex.paths[_pipex.num++], _pipex.args[0]);
	while (access(_pipex.pathname, X_OK) == -1 && _pipex.pathname != NULL)
	{
		free(_pipex.pathname);
		_pipex.pathname = pathname(_pipex.paths[_pipex.num++], _pipex.args[0]);
	}
	if (_pipex.pathname == NULL)
		check_file(_pipex, argv, -1);
	execve(_pipex.pathname, _pipex.args, NULL);
}

int	mid_fork(int argc, char **argv, char **env, t_pipex _pipex)
{
	_pipex.i = 0;
	_pipex.cmd_num = 3;
	_pipex.pipe_num = (argc - 3) - 1;
	int	mid_pipes[_pipex.pipe_num * 2];

	while (_pipex.i < _pipex.cmd_num)
		pipe(mid_pipes + i++);
	while (_pipex.i < _pipex.pipes_num)
	{
		_pipex.middle_pid = fork ()
		if (!_pipex.middle_pid)
			middle_child(argv, env, _pipex, mid_pipes);
		else if (_pipex.middle_pid == -1)
			return (free_all(_pipex, 3), perror("middle fork error"), 1);
		i++;
	}
	return (0);
}

/*
*	(function breakdown)
*	64:	check argc is right	
*	66:	create a pipe
*	68:	fork the process into eldest
*	73:	fork the processes into youngest
*	78:	wait for the last child so as to not have an orphan process
*/
// int	main(int argc, char **argv, char **env)//25
// {	
	// t_pipex	_pipex;
	// int		status;
// 
	// _pipex.num = 0;
    // if (argc < 5)
        // return (0);
    // if (here_doc())
// 
	// if (pipe(_pipex.pipe_fd) == -1)
		// return (perror("pipe error"), 0);
	// _pipex.eldest_pid = fork();
	// if (!_pipex.eldest_pid)
		// eldest(argv, env, _pipex);
	// else if (_pipex.eldest_pid == -1)
		// return (free_all(_pipex, 1), perror("fork1 error"), 0);
	// if (argc > 5)
		// if (mid_fork(argc, argv, env, _pipex));
            // return (0);
	// if (!_pipex.youngest_pid)
		// youngest(argv, env, _pipex);
	// else if (_pipex.youngest_pid == -1)
		// return (free_all(_pipex, 1), perror("fork2 error"), 0);
	// wait(&status);
	// free_all(_pipex, 0);
	// return (0);
// }

int main(int argc, char **argv, char **env)
{
    t_pipex _pipex;
}
