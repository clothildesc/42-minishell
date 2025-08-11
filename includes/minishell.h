/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/08/11 18:06:23 by cscache          ###   ########.fr       */
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
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

/*=============== PARSING =============== */

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_args
{
	char			*arg;
	struct s_args	*next;
}	t_arg;

typedef struct s_redir
{
	char			*redir;
	char			*file;
	int				fd_in;
	int				fd_out;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*name;
	t_arg			*args;
	char			*abs_path;
	t_redir			*fds;
	int				pipefd[2];
	int				exit_status;
	struct s_cmd	*prev;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_ast
{
	t_node_type		node_type;
	t_cmd			*cmds;
	int				prio;
	struct s_ast	*right;
	struct s_ast	*left;
}	t_ast;

/*=============== EXEC =============== */

/*
- Peut-etre plus facile d'utilise des pointeur pour :
	- tokens -> liste chaine donc pointe vers une suite de variable
	- ast -> idem
*/
typedef struct s_shell
{
	t_lexer	lexer;
	t_token	tokens;
	t_ast	ast;
	t_cmd	*cmds;
	t_env	*env;
	int		exit_status;
}	t_shell;

/*=============== FUNCTIONS =============== */

int				get_syntax_error_status(t_token *lst_tokens);

/*-------STRUCT-------*/
void			init_all_structs(t_shell *shell);
t_env			*get_env(char **envp);

/*-------Lexer-------*/
t_token			*ft_lexer(char *input, t_shell *shell);
int				check_if_not_normal_state(t_lexer *lexer);
void			handle_double_quote_state(t_lexer *lexer);
void			handle_single_quote_state(t_lexer *lexer);
void			handle_normal_state(t_lexer *lexer);

/*-------Token-------*/
void			create_token(t_lexer *lexer, bool to_join);
void			add_char(t_list **tmp_token, char c);
void			clear_tokens_lst(t_token **lst);
t_token_type	determine_token_type(t_lexer *lexer);

/*-------AST-------*/
t_ast			*set_ast(t_shell *shell, t_token *lst_tokens);
void			ft_lstadd_redir(t_redir **lst, t_redir *new);
void			ft_lstadd_args(t_arg **lst, t_arg *new);
void			create_redir_lst(t_token *token, t_cmd *cmd);
void			create_args_lst(t_token *token, t_cmd *cmd, t_env *env);
t_cmd			*parse_cmd_name(t_cmd *new, char *cmd_name, t_env *env);
void			free_args(char **result, int i);
bool			is_pipe(t_token *lst_token);
t_token			*find_pipe(t_token *lst_token);

/*-------Env-------*/
void			ft_lstadd_back_env(t_env **lst, t_env *new);

/*
* Les builtin ne sont pas a 100% fonctionel
*/
/*-------Builtin-------*/
void			builtin_env(t_env *env);
void			builtin_unset(t_env **env, char *to_delete);
char			*builtin_expand(char *input, t_env *env);
/* ft_export */
void			builtin_export(t_env *env, char *input);
char			*get_input_value(char *input);
char			*get_input_key(char *input);
int				compare_key(char *env, char *inpt);
t_env			*get_node(t_env **head, char *key);

/*-------Display|TEST-------*/
void	display_lexer_results(t_token *lst_tokens);
void	display_ast_results(t_ast *node, int depth, char branch);

#endif
