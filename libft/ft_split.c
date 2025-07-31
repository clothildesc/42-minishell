/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:23:51 by cscache           #+#    #+#             */
/*   Updated: 2025/07/31 15:10:28 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_WORD_EXPANDs(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (i == 0 || c == s[i - 1])
		{
			if (c != s[i])
				count++;
		}
		i++;
	}
	return (count);
}

static char	*ft_extract_WORD_EXPAND(char const *s, int start, int end)
{
	char	*tab;
	int		i;

	i = 0;
	tab = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!tab)
		return (NULL);
	while (start < end)
	{
		tab[i] = s[start];
		i++;
		start++;
	}
	tab[i] = '\0';
	return (tab);
}

static void	ft_free(char **result, int i)
{
	while (i >= 0)
	{
		free(result[i]);
		i--;
	}
	free(result);
}

static char	**ft_split_fill(char const *s, char **result, char c)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (s[i] && j < ft_count_WORD_EXPANDs(s, c))
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			result[j] = ft_extract_WORD_EXPAND(s, start, i);
			if (!result[j])
			{
				ft_free(result, j);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	result[j] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	result = (char **)malloc(sizeof(char *) * (ft_count_WORD_EXPANDs(s, c) + 1));
	if (!result)
		return (NULL);
	return (ft_split_fill(s, result, c));
}
