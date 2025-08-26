/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/08/26 15:13:42 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libft/libft.h"

/*=============== EXIT CODES =============== */
# define EXIT_SUCCESS 			0
# define EXIT_FAILURE 			1
# define EXIT_SYNTAX_ERROR		2
# define EXIT_CMD_NOT_FOUND		127
# define EXIT_PERMISSION_DENIED 126
# define EXIT_SIGNAL			128
# define EXIT_CTRL_C 			130
# define EXIT_CTRL_D 			131

/*=============== ERRORS =============== */

# define SYNTAX_ERROR_TOKEN "bash: syntax error near unexpected token "
# define SYNTAX_ERROR_KEY_ENV "bash: export: not a valid identifier"
# define ERROR_CD_MANY_ARGS "bash: cd: too many arguments"
# define ERROR_MISSING_FILE "bash: No such file or directory"
# define ERROR_CMD_NOT_FOUND "bash: Command not found"

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
	bool			to_join;
	struct s_args	*next;
}	t_arg;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	t_redir			*redirs;
	char			*abs_path;
	int				fd_in;
	int				fd_out;
	int				fd_heredoc;
	pid_t			pid;
	int				exit_status;
}	t_cmd;

struct	s_ast;

typedef struct s_ast_binary
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast_binary;

typedef struct s_ast_unary_cmd
{
	t_cmd	*cmd;
}	t_ast_unary_cmd;

typedef union u_ast_data
{
	t_ast_binary	binary;
	t_ast_unary_cmd	cmd;
}	t_ast_data;

typedef struct s_ast_unary
{
	t_cmd	*cmd;
}	t_ast_unary;

typedef struct s_ast
{
	t_node_type		node_type;
	t_ast_data		data;
}	t_ast;

/*=============== EXEC =============== */

typedef struct s_shell
{
	t_lexer	lexer;
	t_token	*tokens;
	t_ast	*ast;
	t_env	*env;
	int		prev_status;
	int		status;
}	t_shell;

/*=============== GLOBAL VARIABLE =============== */

extern int	g_exit_status;

/*=============== FUNCTIONS =============== */
/*-------Syntax errors-------*/
int				handle_special_char(t_token *head);
int				get_syntax_error_status(t_token *lst_tokens);
void			get_syntax_errors(t_token *lst_tokens);

/*-------Structs-------*/
void			init_all_structs(t_shell *shell, char **envp);
void			clear_args_lst(t_arg **lst);
void			clear_redirs_lst(t_redir **lst);
void			clear_cmd(t_cmd *cmd);
void			clear_ast(t_ast **ast);
void			free_tab_chars(char **tab);
void			clear_env_lst(t_env **env);
void			clear_lexer_tmp(t_lexer *lexer);
void			clear_shell(t_shell *shell);
void			free_and_exit(t_shell *shell, int exit_code);

/*-------Lexer-------*/
t_token			*ft_lexer(char *input, t_shell *shell);
void			process_normal_state(t_lexer *lexer);

/*-------Token-------*/
void			create_token(t_lexer *lexer, bool to_join);
void			add_char(t_list **tmp_token, char c);
void			clear_tokens_lst(t_token **lst);
t_token_type	determine_token_type(t_lexer *lexer);
char			*create_token_value(t_lexer *lexer);

/*-------AST-------*/
t_ast			*parse_pipe(t_shell *shell, t_token **tokens);
t_ast			*parse_cmd(t_token **tokens, t_env *env);
t_cmd			*parse_cmd_name(t_cmd *new, char *cmd_name, t_env *env);
void			ft_lstadd_args(t_arg **args, t_arg *new);
void			create_args_lst(t_arg **args, t_token *token, t_env *env);
void			lst_args_to_array(t_cmd *cmd, t_arg **args);
void			create_redir_lst(t_token *token, t_cmd *cmd);

/*-------Builtin-------*/
/* env */
t_env			*get_env(char **envp);
void			ft_lstadd_back_env(t_env **lst, t_env *new);
int				builtin_env(t_env *env);
/* unset */
int				builtin_unset(t_env **env, char **args);
/* export */
int				builtin_export(t_env *env, char **args);
int				value_to_append(char *input);
char			*get_input_value(char *input);
char			*get_input_key(char *input);
int				compare_key(char *env, char *inpt);
t_env			*get_node(t_env **head, char *key);
t_env			*create_new_env_node(t_env *new, char *input, char *key);
int				print_env_export(t_env *env);
/* expand */
char			*builtin_expand(char *input, t_env *env);
/* pwd */
int				builtin_pwd(void);
/* cd */
int				builtin_cd(char **args, t_env *env);
/* echo */
int				builtin_echo(char **args, t_shell *shell);
/* exit */
// int				builtin_exit(t_shell *shell, char **args);

/*-------Execution-------*/
/* builtins */
int				is_a_builtin(char *name);
bool			is_parent_builtin(char *name);
int				exec_builtin_simple(t_cmd *cmd, t_shell *shell, \
				int fd_i, int fd_o);
int				exec_builtin_in_parent(t_cmd *cmd, t_shell *shell, \
				int fd_i, int fd_o);
int				execute_builtins(t_cmd *cmd, t_shell *shell);
int				execute_parent_builtins(t_cmd *cmd, t_shell *shell);
/* others cmds */
char			**lst_env_to_array(t_env *env);
int				cmd_not_found(t_cmd *cmd);
int				prepare_cmd(t_cmd *cmd, t_env *env);
int				execute_cmd(t_ast *node, t_shell *shell, int fd_i, int fd_o);
void			execute_ast(t_ast *node, t_shell *shell, int fd_i, int fd_o);
void			execution(t_ast *ast, t_shell *shell);
int				get_exit_code(int status);
/* redir & heredoc */
int				open_infile(char *infile);
int				open_outfile(char *outfile, t_token_type type);
int				prepare_redirections(t_cmd *cmd);
void			manage_dup(t_cmd *cmd, int fd_i, int fd_o);
void			handle_all_heredocs(t_ast *node);

/*-------Display|TEST-------*/
void	display_lexer_results(t_token *lst_tokens);
void	display_ast_results(t_ast *node, int depth, char branch);

#endif
