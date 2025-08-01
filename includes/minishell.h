/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/08/01 12:05:52 by cscache          ###   ########.fr       */
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
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_HERE_DOC,
	TOKEN_APPEND_OUT,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_UNDEFINED
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			to_exp;
	bool			to_join;
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
	bool			to_join;
	char			*input;
}	t_lexer;

/*=============== ENV =============== */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct t_env	*next;
}	t_env;

/*=============== PARSING =============== */

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_ast
{
	t_node_type		node_type;
	t_cmd			*cmds;
	int				prio;
	struct s_ast	*right;
	struct s_ast	*left;
}	t_ast;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	char			*abs_path;
	int				fd_in;
	int				fd_out;
	int				pipefd[2];
	int				exit_status;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

/*=============== EXEC =============== */

typedef struct s_shell
{
	t_lexer	lexer;
	t_token	tokens;
	t_ast	ast;
	t_env	*env;
	t_cmd	*cmds;
	int		exit_status;
}	t_shell;

/*=============== FUNCTIONS =============== */

void			init_all_structs(t_shell *shell);

t_token			*ft_lexer(char *input, t_shell *shell);
int				check_if_not_normal_state(t_lexer *lexer);
void			handle_double_quote_state(t_lexer *lexer);
void			handle_single_quote_state(t_lexer *lexer);
void			handle_normal_state(t_lexer *lexer);

void			create_token(t_lexer *lexer);
void			add_char(t_list **tmp_token, char c);
t_token_type	determine_token_type(t_lexer *lexer);
void			clear_tokens_lst(t_token **lst);

int				get_syntax_error_status(t_token *lst_tokens);
void			set_to_join(t_lexer *lexer);

#endif
