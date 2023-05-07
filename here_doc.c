/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 11:51:54 by djacobs           #+#    #+#             */
/*   Updated: 2023/05/03 11:51:55 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	while (*s1 == *s2 && n - 1)
	{
		s1++;
		s2++;
		n--;
	}
	if ((*s1 - *s2) == -128)
		return (1);
	return (*s1 - *s2);
}

int	here_doc(int argc, char **argv, char **env, t_pipex _pipex)
{
	(void)argv;
	(void)env;
	(void)argc;
	(void)_pipex;
	return(0);
}