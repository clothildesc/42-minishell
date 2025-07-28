/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/07/28 17:47:12 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libft/libft.h"

/*=============== LEXER =============== */

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
	CHAR_DOUBLE_QUOTE,
	CHAR_PIPE,
	CHAR_REDIR_IN,
	CHAR_REDIR_OUT
}	t_char_type;

typedef struct s_lexer
{
	t_state		state;
	t_list		*tmp_token;
	t_token		*lst_tokens;
	int			pos;
	char		**input;
}	t_lexer;

typedef enum e_token_type
{
	WORD,
	WORD_SINGLE_QUOTE,
	WORD_DOUBLE_QUOTE,
	PIPE,
	HERE_DOC,
	APPEND,
	REDIR_IN,
	REDIR_OUT
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/*=============== ENV =============== */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct t_env	*next;
}	t_env;

/*=============== EXEC =============== */

typedef struct s_cmd
{
	// ????
}	t_cmd;

typedef struct s_shell
{
	t_env	*env;
	t_cmd	*cmds;
	int		exit_status;
}	t_shell;

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