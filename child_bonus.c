/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:14:03 by djacobs           #+#    #+#             */
/*   Updated: 2023/05/03 13:14:04 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	gnl(char **line)
{
	char	*buf;
	int		n;
	char	c;
	int		i;

	i = 0;
	buf = malloc(sizeof(char) * BUFSIZ);
	n = read(STDIN_FILENO, &c, 1);
	while(n && c != '\n' && c != '\0')
	{
		if (c != 'n', c != '\0')
			buf[i] = c;		
		n = read(STDIN_FILENO, &c, 1);
		i++;
	}
	buf[i] = '\n';
	buf[++i] = 0;
	*line = buf;
	free(buf);
	return (n);
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


void	execute(char *argv, char **env, t_pipex _pipex)
{
	int	i;

	i = 0;
	_pipex.paths = ft_split(envstr(env, "PATH"), ':');
	_pipex.args = ft_split(argv, ' ');
	if (_pipex.paths == NULL)
		_pipex.paths = ft_split("/usr/bin:/usr/sbin:/bin:/sbin", ':');
	while (_pipex.paths[i] != NULL)
	{
		_pipex.pathname = pathname(_pipex.paths[i++], _pipex.args[0]);
		if (access(_pipex.pathname, X_OK) == 0 && _pipex.pathname != NULL)
			break;
		else if (_pipex.paths == NULL)
		{
			free_all(_pipex, 0);
			perror("Error path not found :");
			exit(EXIT_FAILURE);
		}
		free(_pipex.pathname);
	}
	execve(_pipex.pathname, _pipex.args, NULL);
}

int	child_process(char *argv, char **env, t_pipex _pipex)
{
	pid_t	pid;

	if (pipe(_pipex.pipe_fd) == -1)
			return (perror ("pipe error"), 0);
	pid = fork();
	if (pid == -1)
		return (perror("fork error :"), 0);
	if (pid == 0)
	{
		dup2(_pipex.pipe_fd[1], STDOUT_FILENO);
		free_all(_pipex, 1);
		execute(argv, env, _pipex);
	}
	else
	{
		close(_pipex.pipe_fd[1]);
		dup2(_pipex.pipe_fd[0], STDIN_FILENO);
		close (_pipex.pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	return (0);
}
