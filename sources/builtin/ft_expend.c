/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expend.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:50:06 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/11 10:17:02 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static bool	check_arg(char *input)
// {
// 	int	i;

// 	i = 0;
// 	while (input[i])
// 	{
// 		if (input[0] != '$')
// 			return (false);
// 		if (!ft_isuper(input[i]))
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

// char	*ft_expend(char *input, int fd, t_env *env)
// {
// 	char	*ret;

// 	if (!check_arg(input))
// 		return (NULL);
// 	ret = search_result(&env, &input[1]);
// 	if (!ret)
// 		return (NULL);
// 	return (ret);
// }
