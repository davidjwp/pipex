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

void	free_all(t_pipex _pipex, int fds)
{
	int	i;

	i = 0;
	if (_pipex.pathname != NULL)
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

int main(int argc, char **argv, char **env)
{
	t_pipex	_pipex;
	int		i;

	i = 0;
	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0) 
			return (here_doc(argc, argv, env, _pipex));
		else
		{
			_pipex.i = open_file(argv[1], 1);
			_pipex.o = open_file(argv[argc - 1], 0);
			dup2(_pipex.i, STDIN_FILENO);
		}
		if (!_pipex.i || !_pipex.o)
			return (close (_pipex.i), close (_pipex.o), perror("Error :"),0);
		while (i < (argc - 2)) 
			child_process(argv[i++], env, _pipex);
		dup2(_pipex.o, STDOUT_FILENO);
		execute(argv[i], env, _pipex);
	}
	return (wait(NULL), free_all(_pipex, 1), 0);
}
