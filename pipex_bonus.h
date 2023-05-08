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
	int		index;
	char	**args;
	char	**paths;
	int		cmd_num;
	// int		scd_file;
	// int		pipe_num;
	char	*pathname;
	int		*stat_loc;
	int		pipe_fd[2];
	char	buf[BUFSIZ];
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
void	free_all(t_pipex _pipex, int fds);
int		check_file(t_pipex _pipex, char **argv, int i);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		child_process(char *argv, char **env, t_pipex _pipex);
void	execute(char *argv, char **env, t_pipex _pipex);
int		gnl(char **line);

#endif
