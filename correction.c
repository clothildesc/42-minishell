/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   correction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 11:35:16 by cscache           #+#    #+#             */
/*   Updated: 2025/09/10 11:35:17 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void display_tokens(t_shell *shell)
// {
// 	t_token	*current;
// 	int	i = 1;

// 	current = shell->tokens;
// 	while (current)
// 	{
// 		if (current->value[0] == '\0')
// 			ft_printf("[%d] = vide |  to_join = %d | to_exp = %d | state = %d\n", i, current->value, current->to_join, current->to_exp, current->state);
// 		else
// 			ft_printf("[%d] = %s |  to_join = %d | to_exp = %d | state = %d\n", i, current->value, current->to_join, current->to_exp, current->state);
// 		i++;
// 		current = current->next;
// 	}
// }