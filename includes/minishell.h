/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:00:27 by cscache           #+#    #+#             */
/*   Updated: 2025/07/28 12:13:30 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libft/libft.h"

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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct t_env	*next;
}	t_env;

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

#endif