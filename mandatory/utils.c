/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:26:48 by djacobs           #+#    #+#             */
/*   Updated: 2023/04/24 18:26:49 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*ft_word_cpy(char const *s, char c)
{
	char	*word_cpy;
	int		word_len;
	int		index;

	index = 0;
	word_len = 0;
	while (s[word_len] != c && s[word_len])
		word_len++;
	word_cpy = (char *)malloc(word_len + 1);
	while (index < word_len)
	{
		word_cpy[index] = *s++;
		index++;
	}
	word_cpy[index] = '\0';
	return (word_cpy);
}

static int	ft_word_count(char const *s, char c)
{
	int	word_count;

	word_count = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (!*s)
			return (word_count);
		word_count++;
		while (*s != c && *s)
			s++;
	}
	return (word_count);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		index;

	index = 0;
	if (!s)
		return (NULL);
	split = (char **)malloc(sizeof (char *) * (ft_word_count(s, c) + 1));
	if (!split)
		return (NULL);
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
			split[index] = ft_word_cpy(s, c);
		index++;
		while (*s != c && *s)
			s++;
	}
	split[index] = NULL;
	return (split);
}

/*
*	this function will find the right environment variable you are looking for
*	this variable could be anything though for this exercise i'll be PATH
*
*	if the variable isn't found it will return NULL
*/
char	*envstr(char **envector, const char *envariable)
{
	int	i;
	int	i2;
	int	n;

	n = 0;
	i = 0;
	i2 = 0;
	while (envariable[n])
		n++;
	while (envector[i] != NULL)
	{
		while (envector[i][i2] == envariable[i2])
			i2++;
		if (i2 == n)
		{
			while (n--)
				envector[i]++;
			return (++envector[i]);
		}
		i2 = 0;
		i++;
	}
	return (NULL);
}

size_t	ft_strlen(const char *s)
{
	unsigned int	lenght;

	lenght = 0;
	while (s[lenght])
		lenght++;
	return (lenght);
}
