/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:10:35 by djacobs           #+#    #+#             */
/*   Updated: 2023/05/03 13:10:37 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

/*
*	int		i;				/infile file descriptor
*	int		o;				/outfile file descriptor
*	pid_t	pid;			/process id
*	int		_int;			/integer used as index, naming scheme to shorten name
*	char	**args;			/execve command arguments
*	char	**paths;		/PATH env in list form
*	char	*pathname;		/execve pathname
*	int		pipe_fd[2];		/pipe file descriptors
*	char	input_file		/string literal of the input file 
*/
typedef struct t_PIPEX
{
	int		i;
	int		o;
	pid_t	pid;
	int		_int;
	char	**args;
	char	**paths;
	char	*pathname;
	int		pipe_fd[2];
	char	*input_file;
}				t_pipex;

//children
int		child_process(char *argv, char **env, t_pipex _pipex);
void	execute(char *argv, char **env, t_pipex _pipex);
char	*envstr(char **envector, const char *envariable);
char	*pathname(char *path, char *name);
char	**ft_split(char const *str, char c);

//utils
size_t	ft_strlen(const char *str);
void	free_all(t_pipex _pipex, int fds);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		gnl(char **line);

#endif
