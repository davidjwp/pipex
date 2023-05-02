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

/*
*	int		i;				/infile file descriptor
*	int		o;				/outfile file descriptor
*	int		num;			/serves as index
*	char	**args;			/execve command arguments
*	char	**paths;		/PATH env in list form
*	char	*pathname;		/execve pathname
*	int		*stat_loc;		/stat_loc for waitpid()
*	int		pipe_fd[2];		/pipe file descriptors
*	int		eldest_pid;		/process id of eldest child
*	char 	buf[BUFSIZ];	/buffer for final cmd redirection
*	int		youngest_pid	/process if of youngest child
*/
typedef struct t_PIPEX
{
	int		i;
	int		o;
	int		num;
	char	**args;
	char	**paths;
	char	*pathname;
	int		*stat_loc;
	int		pipe_fd[2];
	int		eldest_pid;
	char	buf[BUFSIZ];
	int		youngest_pid;
}				t_pipex;

//children
void	eldest(char **argv, char **env, t_pipex _pipex);
void	youngest(char **argv, char **env, t_pipex _pipex);

//utils
char	**ft_split(char const *str, char c);
char	*envstr(char **envector, const char *envariable);
char	*pathname(char *path, char *name);
size_t	ft_strlen(const char *str);
int		check_cmd(t_pipex _pipex);
int		free_all(t_pipex _pipex, int mode);

#endif
