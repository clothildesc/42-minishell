/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cscache <cscache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:19 by cscache           #+#    #+#             */
/*   Updated: 2025/07/29 17:39:12 by cscache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

// Ajoutez une fonction pour détecter les opérateurs multi-caractères
// Au lieu de retourner un char **, retournez une liste chaînée de t_token

// Algorithme de tokenisation amélioré
	// Skip whitespace au début
	// Identifier le type de token :
		// Si c'est un opérateur → créer un token d'opérateur
		// Si c'est une quote → traiter la chaîne quotée
		// Sinon → traiter comme un mot
	// Pour les opérateurs :
		// Vérifier s'il y a un deuxième caractère pour les opérateurs composés
		// Créer le token avec le bon type
	// Pour les mots :
		// Continuer jusqu'à trouver un espace, une quote ou un opérateur

// Gestion d'erreurs robuste
	// Quotes non fermées
	// Opérateurs invalides
	// Allocation mémoire échouée
	// Syntaxe invalide

// Logique par état :
	// STATE_NORMAL :
		// Espaces → séparent les tokens
		// Quotes → changent d'état
		// Opérateurs (|, <, >) → créent des tokens d'opérateur
		// Caractères normaux → ajoutés au token courant
	// STATE_SINGLE_QUOTE :
		// Tout caractère → ajouté littéralement (même $, \, etc.)
		// Quote simple → retour à STATE_NORMAL
		// Aucun échappement possible
	// STATE_DOUBLE_QUOTE :
		// Quote double → retour à STATE_NORMAL
		// $ → début d'expansion de variable
		// \ → échappement du caractère suivant (optionnel)
		// Autres caractères → ajoutés littéralement

t_char_type	classify_char_type(char c)
{
	if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '\'')
		return (CHAR_SINGLE_QUOTE);
	else if (c == '"')
		return (CHAR_DOUBLE_QUOTE);
	else if (c == '|')
		return (CHAR_PIPE);
	else if (c == '<')
		return (CHAR_REDIR_IN);
	else if (c == '>')
		return (CHAR_REDIR_OUT);
	return (CHAR_NORMAL);
}

void	add_char(t_list **tmp_token, char c)
{
	t_list	*new;
	char	*new_char;

	new_char = malloc(sizeof(char));
	if (!new_char)
		return ;
	*new_char = c;
	new = ft_lstnew(new_char);
	if (!new)
	{
		free(new_char);
		return ;
	}
	ft_lstadd_back(tmp_token, new);
}

char	*create_token_value(t_lexer *lexer)
{
	char	*token_value;
	int		i;

	if (!(lexer->tmp_token))
		return (NULL);
	token_value = malloc(sizeof(char) * (ft_lstsize(lexer->tmp_token) + 1));
	if (!token_value)
		return (NULL);
	i = 0;
	while (lexer->tmp_token)
	{
		token_value[i] = *(char *)lexer->tmp_token->content;
		i++;
		lexer->tmp_token = lexer->tmp_token->next;
	}
	token_value[i] = 0;
	return (token_value);
}

void	add_to_lst_tokens(t_token **lst, t_token *new)
{
	t_token	*last;

	last = NULL;
	if (lst && new)
	{
		if (*lst)
		{
			last = *lst;
			while (last->next)
				last = last->next;
			last->next = new;
		}
		else
			*lst = new;
	}
}

void	finish_token_with_type(t_lexer *lexer)
{
	char	*token_value;
	t_token	*new_token;

	if (lexer->tmp_token)
	{
		token_value = create_token_value(lexer);
		if (token_value)
		{
			new_token = malloc(sizeof(t_token));
			if (!new_token)
				free(token_value);
			new_token->value = token_value;
			new_token->type = lexer->type;
			new_token->next = NULL;
			add_to_lst_tokens(&lexer->tokens, new_token);
		}
		ft_lstclear(&lexer->tmp_token, free);
		lexer->tmp_token = NULL;
		lexer->type = UNDEFINED;
		lexer->state = STATE_NORMAL;
	}
}

void	clear_tokens_lst(t_token **lst)
{
	t_token	*last;

	if (lst)
	{
		while (*lst)
		{
			last = (*lst)->next;
			free((*lst)->value);
			free(*lst);
			*lst = last;
		}
	}
}

// char	**lst_to_array(t_list *lst_tokens)
// {
// 	char	**tokens;
// 	int		i;
// 	t_list	*current;
// 	t_list	*to_free;

// 	tokens = malloc(sizeof(char *) * (ft_lstsize(lst_tokens) + 1));
// 	if (!tokens)
// 		return (NULL);
// 	i = 0;
// 	current = lst_tokens;
// 	while (current)
// 	{
// 		tokens[i] = (char *)current->content;
// 		current = current->next;
// 		i++;
// 	}
// 	tokens[i] = NULL;
// 	while (lst_tokens)
// 	{
// 		to_free = lst_tokens;
// 		lst_tokens = lst_tokens->next;
// 		free(to_free);
// 	}
// 	return (tokens);
// }
