/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:51:53 by cscache           #+#    #+#             */
/*   Updated: 2025/07/28 12:13:03 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LEXER_H
# define FT_LEXER_H

# include <unistd.h>
# include <stdlib.h>

typedef struct s_list	t_list;

typedef enum e_state
{
	STATE_NORMAL,
	STATE_SINGLE_QUOTE,
	STATE_DOUBLE_QUOTE
}	t_state;

typedef enum e_char_type
{
	CHAR_NORMAL,
	CHAR_SPACE,
	CHAR_SINGLE_QUOTE,
	CHAR_DOUBLE_QUOTE
	CHAR_PIPE,
	CHAR_REDIR_IN,
	CHAR_REDIR_OUT
	CHAR_HEREDOC,
	CHAR_APPEND,
}	t_char_type;

typedef struct s_lexer
{
	t_state		state;
	t_list		*tmp_token;
	t_list		*lst_tokens;
	int			pos;
	const char	**input;
}	t_lexer;

t_char_type	classify_char_type(char c);
void		add_char(t_list **tmp_token, const char *c);
char		*create_token(t_list *tmp_token);
void		finish_token(t_list **tmp_token, t_list **lst_tokens);
char		**lst_to_array(t_list *lst_tokens);
void		handle_single_quote_state(t_lexer *l);
void		handle_double_quote_state(t_lexer *l);
void		handle_space_state(t_lexer *l);
void		handle_normal_state(t_lexer *l);
void		init_struct_lexer(t_lexer *l);
char		**ft_lexer(char **input);
int			check_if_not_normal_state(t_lexer *l);

#endif