/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 10:01:29 by djacobs           #+#    #+#             */
/*   Updated: 2023/04/22 10:01:31 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

/*
*	int		in;				/infile file descriptor
*	int		out;			/outfile file descriptor
*	int		error;			/file descriptor error 
*	char	**args;			/execve command arguments
*	char	**paths;		/PATH env in list form
*	char	*pathname;		/execve pathname
*	int		*stat_loc;		/stat_loc for waitpid()
*	int		pipe_fd[2];		/pipe
*	int		eldest_pid;		/process id of eldest child
*	char 	buf[BUFSIZ];	/buffer for final cmd redirection
*/
typedef struct t_PIPEX
{
	int		i;
	int		in;
	int		out;
	int		error;
	char	**args;
	char	**paths;
	char	*pathname;
	int		*stat_loc;
	int		pipe_fd[2];
	int		eldest_pid;
	char	buf[BUFSIZ];
	int		youngest_pid;
}				t_pipex;

void	eldest(char **argv, char **env, t_pipex _pipex);
void	youngest(char **argv, char **env, t_pipex _pipex);
char	**ft_split(char const *str, char c);
char	*envstr(char **envector, const char *envariable);
char	*pathname(char *path, char *name);
size_t	ft_strlen(const char *str);

#endif
