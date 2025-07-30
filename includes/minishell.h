/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/07/30 10:58:49 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include "../libft/libft.h"

/*=============== LEXER =============== */

typedef enum e_state
{
	STATE_NORMAL,
	STATE_SINGLE_QUOTE,
	STATE_DOUBLE_QUOTE
}	t_state;

typedef enum e_token_type
{
	WORD,
	WORD_SINGLE_QUOTE,
	WORD_DOUBLE_QUOTE,
	PIPE,
	HERE_DOC,
	APPEND_OUT,
	REDIR_IN,
	REDIR_OUT,
	UNDEFINED
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	t_state			state;
	t_list			*tmp_token;
	t_token			*tokens;
	int				pos;
	int				single_quote;
	int				double_quote;
	char			*input;
}	t_lexer;

/*=============== ENV =============== */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct t_env	*next;
}	t_env;

/*=============== EXEC =============== */

// typedef struct s_cmd
// {
// 	// ????
// }	t_cmd;

typedef struct s_shell
{
	t_env	*env;
	// t_cmd	*cmds;
	int		exit_status;
}	t_shell;

void			init_struct_lexer(t_lexer *lexer);
int				check_if_not_normal_state(t_lexer *lexer);
t_token			*ft_lexer(char *input);
void			add_char(t_list **tmp_token, char c);
char			*create_token_value(t_lexer *lexer);
void			add_to_lst_tokens(t_token **lst, t_token *new);
t_token_type	determine_token_type(t_lexer *lexer);
void			create_token(t_lexer *lexer);
void			clear_tokens_lst(t_token **lst);

#endif
