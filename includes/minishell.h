/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/07/31 15:12:31 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include "../libft/libft.h"

/*=============== ERRORS =============== */

# define SYNTAX_ERROR_PIPE "bash: syntax error near unexpected token '|'"
# define SYNTAX_ERROR_REDIR "bash: syntax error near unexpected token 'newline'"

/*=============== LEXER =============== */

typedef enum e_state
{
	STATE_NORMAL,
	STATE_SINGLE_QUOTE,
	STATE_DOUBLE_QUOTE
}	t_state;

typedef enum e_token_type
{
	WORD_EXPAND,
	WORD_NO_EXPAND,
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
	bool			to_exp;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	t_state			state;
	t_list			*tmp_token;
	t_token			*tokens;
	int				pos;
	bool			to_exp;
	// int				single_quote;
	// int				double_quote;
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

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				fd_in;
	int				fd_out;
	int				pipefd[2];
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_lexer	lexer;
	t_token	tokens;
	t_env	*env;
	t_cmd	*cmds;
	int		exit_status;
}	t_shell;

/*=============== FUNCTIONS =============== */

void			init_all_structs(t_shell *shell);

int				check_if_not_normal_state(t_lexer *lexer);
t_token			*ft_lexer(char *input, t_shell *shell);
void			add_char(t_list **tmp_token, char c);
char			*create_token_value(t_lexer *lexer);
void			add_to_lst_tokens(t_token **lst, t_token *new);
t_token_type	determine_token_type(t_lexer *lexer);
void			create_token(t_lexer *lexer);
void			clear_tokens_lst(t_token **lst);

int				get_syntax_error_status(t_token *lst_tokens);

#endif
