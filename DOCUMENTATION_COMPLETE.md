# Documentation Complète - Minishell

## Table des Matières

1. [Vue d'ensemble](#vue-densemble)
2. [Architecture du Projet](#architecture-du-projet)
3. [Pipeline d'Exécution](#pipeline-dexécution)
4. [Structures de Données](#structures-de-données)
5. [Lexeur (Lexer)](#lexeur-lexer)
6. [Parseur (Parser)](#parseur-parser)
7. [Arbre Syntaxique Abstrait (AST)](#arbre-syntaxique-abstrait-ast)
8. [Exécution des Commandes](#exécution-des-commandes)
9. [Commandes Intégrées (Builtins)](#commandes-intégrées-builtins)
10. [Redirections et Heredocs](#redirections-et-heredocs)
11. [Gestion de l'Environnement](#gestion-de-lenvironnement)
12. [Gestion de la Mémoire](#gestion-de-la-mémoire)
13. [Cas Particuliers et Problèmes Résolus](#cas-particuliers-et-problèmes-résolus)
14. [Codes d'Erreur](#codes-derreur)
15. [Tests et Validation](#tests-et-validation)

---

## Vue d'ensemble

Ce projet implémente un shell Unix minimaliste en C, respectant les normes de l'école 42. Le shell est capable de :

- **Exécuter des commandes** externes et intégrées
- **Gérer les pipes** (`|`) pour chaîner les commandes
- **Gérer les redirections** (`<`, `>`, `>>`) et heredocs (`<<`)
- **Expansion de variables** d'environnement (`$VAR`)
- **Gestion des guillemets** simples et doubles
- **Historique des commandes** avec readline
- **Signaux** (Ctrl+C, Ctrl+D, Ctrl+\)

### Fonctionnalités principales

```bash
minishell> ls -la | grep .c > files.txt
minishell> cat << EOF | wc -l
minishell> export VAR="hello world"
minishell> echo $VAR
minishell> cd .. && pwd
```

---

## Architecture du Projet

### Structure des Fichiers

```
minishell/
├── includes/
│   └── minishell.h          # Définitions, structures, prototypes
├── libft/                   # Bibliothèque personnelle
├── sources/
│   ├── main.c              # Boucle principale du shell
│   ├── display.c           # Fonctions de debug/affichage
│   ├── builtin/            # Commandes intégrées
│   │   ├── builtin_cd.c
│   │   ├── builtin_echo.c
│   │   ├── builtin_env.c
│   │   ├── builtin_exit.c
│   │   ├── builtin_export.c
│   │   ├── builtin_expand.c
│   │   ├── builtin_pwd.c
│   │   ├── builtin_unset.c
│   │   └── builtin_*.c
│   ├── exec/               # Exécution des commandes
│   │   ├── exec_cmd.c
│   │   ├── exec_builtins.c
│   │   ├── exec_heredoc.c
│   │   ├── exec_redirs.c
│   │   └── exec_*.c
│   ├── lexer/              # Analyse lexicale
│   │   ├── lexer.c
│   │   ├── lexer_tokens.c
│   │   ├── lexer_utils.c
│   │   └── lexer_check_errors.c
│   ├── parsing/            # Analyse syntaxique
│   │   ├── parsing_cmd.c
│   │   ├── parsing_pipe.c
│   │   └── parsing_*.c
│   └── structs/            # Gestion des structures
│       ├── init_structs.c
│       ├── free_structs.c
│       └── free_cmds.c
```

---

## Pipeline d'Exécution

### Flux Principal

```c
int execute_shell(char *input, t_shell *shell)
{
    shell->tokens = NULL;
    shell->ast = NULL;
    
    // 1. LEXER : Analyse lexicale
    shell->tokens = ft_lexer(input, shell);
    
    // 2. VALIDATION : Vérification syntaxique
    shell->exit_status = get_syntax_error_status(shell->tokens);
    if (shell->exit_status != EXIT_SUCCESS)
        return cleanup_and_exit(shell);
    
    // 3. PARSER : Construction de l'AST
    shell->ast = parse_pipeline(shell, &shell->tokens);
    
    // 4. EXECUTION : Exécution de l'AST
    if (shell->ast->node_type == NODE_CMD)
        shell->exit_status = execute_command(shell);
    
    // 5. CLEANUP : Nettoyage mémoire
    cleanup_shell(shell);
    return shell->exit_status;
}
```

### Étapes Détaillées

1. **Lexer** : `input` → `tokens[]`
2. **Validator** : Vérification syntaxique
3. **Parser** : `tokens[]` → `AST`
4. **Executor** : `AST` → Exécution
5. **Cleanup** : Libération mémoire

---

## Structures de Données

### Structure Principale : t_shell

```c
typedef struct s_shell {
    t_lexer     lexer;      // Analyseur lexical
    t_token     *tokens;    // Liste de tokens
    t_ast       *ast;       // Arbre syntaxique
    t_env       *env;       // Variables d'environnement
    int         exit_status; // Code de sortie
} t_shell;
```

### Structure de Token

```c
typedef struct s_token {
    char            *value;    // Valeur du token
    t_token_type    type;      // Type (WORD, PIPE, REDIR...)
    bool            to_exp;    // Expansion nécessaire ?
    bool            to_join;   // Jointure avec token suivant ?
    struct s_token  *prev;     // Token précédent
    struct s_token  *next;     // Token suivant
} t_token;
```

### Types de Tokens

```c
typedef enum e_token_type {
    TOKEN_WORD,         // Mots/arguments
    TOKEN_PIPE,         // |
    TOKEN_HERE_DOC,     // <<
    TOKEN_APPEND_OUT,   // >>
    TOKEN_REDIR_IN,     // <
    TOKEN_REDIR_OUT     // >
} t_token_type;
```

### Structure de Commande

```c
typedef struct s_cmd {
    char        *name;          // Nom de la commande
    char        **args;         // Arguments
    t_redir     *redirs;        // Redirections
    char        *abs_path;      // Chemin absolu de l'exécutable
    int         fd_in;          // File descriptor d'entrée
    int         fd_out;         // File descriptor de sortie
    int         fd_heredoc;     // File descriptor heredoc
    pid_t       pid;            // Process ID
    int         exit_status;    // Code de sortie
} t_cmd;
```

### Structure d'Environnement

```c
typedef struct s_env {
    char            *key;       // Nom de la variable
    char            *value;     // Valeur de la variable
    struct s_env    *prev;      // Élément précédent
    struct s_env    *next;      // Élément suivant
} t_env;
```

---

## Lexeur (Lexer)

### Fonctionnement

Le lexeur (`ft_lexer`) analyse caractère par caractère l'entrée utilisateur pour créer une liste de tokens.

### États du Lexeur

```c
typedef enum e_state {
    STATE_NORMAL,           // État normal
    STATE_SINGLE_QUOTE,     // Dans des guillemets simples
    STATE_DOUBLE_QUOTE      // Dans des guillemets doubles
} t_state;
```

### Algorithme Principal

```c
t_token *ft_lexer(char *input, t_shell *shell)
{
    t_lexer lexer = shell->lexer;
    lexer.state = STATE_NORMAL;
    lexer.input = input;
    lexer.to_exp = true;
    lexer.to_join = 0;
    
    while (lexer.input[lexer.pos]) {
        process_current_char(&lexer);
        (lexer.pos)++;
    }
    
    if (check_if_not_normal_state(&lexer))
        return NULL;
        
    create_token(&lexer, true);
    return lexer.tokens;
}
```

### Gestion des Guillemets

- **Guillemets simples** (`'`) : Pas d'expansion de variables
- **Guillemets doubles** (`"`) : Expansion de variables autorisée
- **Échappement** : Non implémenté (pas requis par le sujet)

### Cas Particuliers

```bash
# Guillemets simples - pas d'expansion
echo '$HOME'        # Affiche: $HOME

# Guillemets doubles - expansion
echo "$HOME"        # Affiche: /home/user

# Jointure de tokens
echo hello"world"   # Résultat: helloworld
```

---

## Parseur (Parser)

### Construction de l'AST

Le parseur transforme la liste de tokens en arbre syntaxique abstrait (AST).

### Types de Nœuds

```c
typedef enum e_node_type {
    NODE_CMD,       // Nœud commande
    NODE_PIPE       // Nœud pipe
} t_node_type;
```

### Structure d'AST

```c
typedef struct s_ast {
    t_node_type     node_type;  // Type du nœud
    t_ast_data      data;       // Données (union)
} t_ast;

typedef union u_ast_data {
    t_ast_binary    binary;     // Pour les pipes (left/right)
    t_ast_unary_cmd cmd;        // Pour les commandes
} t_ast_data;
```

### Algorithme de Parsing

```c
t_ast *parse_pipeline(t_shell *shell, t_token **tokens)
{
    t_ast *left = parse_cmd(tokens, shell->env);
    
    while (*tokens && (*tokens)->type == TOKEN_PIPE) {
        *tokens = (*tokens)->next;
        t_ast *right = parse_cmd(tokens, shell->env);
        t_ast *new_pipe = create_pipe_node(left, right);
        left = new_pipe;
    }
    
    return left;
}
```

### Exemple d'AST

Pour la commande : `ls -la | grep .c | wc -l`

```
        PIPE
       /    \
    PIPE     CMD(wc -l)
   /    \
CMD(ls -la) CMD(grep .c)
```

---

## Arbre Syntaxique Abstrait (AST)

### Avantages de l'AST

1. **Structure claire** : Représentation hiérarchique
2. **Extensibilité** : Facile d'ajouter de nouveaux opérateurs
3. **Débogage** : Visualisation de la structure
4. **Exécution** : Traversée récursive naturelle

### Traversée de l'AST

```c
int execute_ast(t_ast *node)
{
    if (!node)
        return EXIT_SUCCESS;
        
    if (node->node_type == NODE_PIPE) {
        // Exécuter pipe : left | right
        return execute_pipe(node->data.binary.left, 
                           node->data.binary.right);
    } else if (node->node_type == NODE_CMD) {
        // Exécuter commande simple
        return execute_command(node->data.cmd.cmd);
    }
    
    return EXIT_FAILURE;
}
```

---

## Exécution des Commandes

### Types d'Exécution

1. **Builtins Parent** : Exécutés dans le processus parent
2. **Builtins Child** : Exécutés dans un processus enfant
3. **Commandes Externes** : Exécutées via fork/exec

### Classification des Builtins

```c
bool is_parent_builtin(char *name)
{
    if (!name) return false;
    
    // Ces commandes modifient l'état du shell parent
    if (!ft_strcmp(name, "cd")) return true;
    if (!ft_strcmp(name, "exit")) return true;
    if (!ft_strcmp(name, "export")) return true;
    if (!ft_strcmp(name, "unset")) return true;
    
    return false;
}
```

### Algorithme d'Exécution

```c
int execute_command(t_shell *shell)
{
    t_cmd *cmd = shell->ast->data.cmd.cmd;
    
    if (!cmd->name)
        return EXIT_SUCCESS;
    
    // 1. Traiter les heredocs en premier
    if (handle_all_heredocs(shell->ast) == -1)
        return EXIT_FAILURE;
    
    // 2. Vérifier si c'est un builtin
    if (is_a_builtin(cmd->name)) {
        if (is_parent_builtin(cmd->name))
            return exec_builtin_in_parent(cmd, shell);
        return exec_builtin_simple(cmd, shell);
    }
    
    // 3. Exécuter commande externe
    return fork_and_execute(cmd, shell);
}
```

### Fork et Exec

```c
static int fork_and_execute(t_cmd *cmd, t_shell *shell)
{
    char **env_array = lst_env_to_array(shell->env);
    
    // Préparer la commande (résolution du PATH)
    int status = prepare_cmd(cmd, shell->env);
    if (status != EXIT_SUCCESS)
        return status;
    
    // Fork et exec
    execute_child(cmd, env_array);
    
    // Attendre la fin du processus enfant
    int exit_code;
    waitpid(cmd->pid, &status, 0);
    exit_code = get_exit_code(status);
    
    free_tab_chars(env_array);
    return exit_code;
}
```

---

## Commandes Intégrées (Builtins)

### Liste Complète

| Commande | Type | Description |
|----------|------|-------------|
| `cd` | Parent | Change de répertoire |
| `echo` | Child | Affiche du texte |
| `env` | Child | Affiche l'environnement |
| `exit` | Parent | Quitte le shell |
| `export` | Parent | Exporte des variables |
| `pwd` | Child | Affiche le répertoire courant |
| `unset` | Parent | Supprime des variables |

### Implémentation : echo

```c
int builtin_echo(char **args)
{
    bool option = false;    // Option -n
    bool first = true;
    int i = 1;
    
    // Traiter les options -n (multiples)
    while (args && args[i] && is_n_option(args[i])) {
        option = true;
        i++;
    }
    
    // Afficher les arguments
    while (args && args[i]) {
        if (!first)
            ft_printf(" ");
        ft_printf("%s", args[i]);
        first = false;
        i++;
    }
    
    // Nouvelle ligne sauf si -n
    if (!option)
        ft_printf("\n");
        
    return EXIT_SUCCESS;
}
```

### Implémentation : cd

```c
int builtin_cd(char **args, t_env *env)
{
    char *path;
    
    if (!args || !args[0])
        return EXIT_SUCCESS;
        
    // Trop d'arguments
    if (args[1] && args[2]) {
        ft_putendl_fd(ERROR_CD_MANY_ARGS, 2);
        return EXIT_FAILURE;
    }
    
    // Pas d'argument = HOME
    if (!args[1]) {
        path = get_env_value(env, "HOME");
        if (!path) {
            ft_putendl_fd("bash: cd: HOME not set", 2);
            return EXIT_FAILURE;
        }
    } else {
        path = args[1];
    }
    
    // Changer de répertoire
    if (chdir(path) == -1) {
        perror("cd");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
```

### Implémentation : export

```c
int builtin_export(t_env *env, char **args)
{
    // Sans arguments : afficher toutes les variables
    if (!args || !args[1])
        return print_env_export(env);
    
    int exit_code = EXIT_SUCCESS;
    int i = 1;
    
    while (args[i]) {
        char *key = get_input_key(args[i]);
        if (!key)
            return EXIT_SUCCESS;
            
        // Valider le nom de la clé
        if (!key_name_is_valid(key)) {
            exit_code = EXIT_FAILURE;
        } else {
            // Créer ou mettre à jour la variable
            t_env *new = find_or_create_env(env, args[i], key);
            if (new)
                ft_lstadd_back_env(&env, new);
        }
        
        free(key);
        i++;
    }
    
    return exit_code;
}
```

---

## Redirections et Heredocs

### Types de Redirections

| Opérateur | Description | Exemple |
|-----------|-------------|---------|
| `<` | Redirection d'entrée | `cmd < file` |
| `>` | Redirection de sortie | `cmd > file` |
| `>>` | Ajout en fin de fichier | `cmd >> file` |
| `<<` | Heredoc | `cmd << EOF` |

### Gestion des Redirections

```c
int prepare_redirections(t_cmd *cmd)
{
    t_redir *current = cmd->redirs;
    
    while (current) {
        if (current->type == TOKEN_HERE_DOC) {
            close_if_open(cmd->fd_in);
            cmd->fd_in = cmd->fd_heredoc;
        }
        else if (current->type == TOKEN_REDIR_IN) {
            close_if_open(cmd->fd_in);
            cmd->fd_in = open_infile(current->target);
            if (cmd->fd_in == -1)
                return -1;
        }
        else if (current->type == TOKEN_REDIR_OUT || 
                 current->type == TOKEN_APPEND_OUT) {
            close_if_open(cmd->fd_out);
            cmd->fd_out = open_outfile(current->target, current->type);
            if (cmd->fd_out == -1)
                return -1;
        }
        current = current->next;
    }
    
    return 0;
}
```

### Application des Redirections

```c
void apply_redirections(t_cmd *cmd)
{
    if (cmd->fd_in != -1) {
        if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
            perror("dup2");
        close(cmd->fd_in);
    }
    
    if (cmd->fd_out != -1) {
        if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
            perror("dup2");
        close(cmd->fd_out);
    }
}
```

### Heredocs

Les heredocs créent un fichier temporaire :

```c
static int create_here_doc(char *limiter)
{
    // Créer un nom de fichier unique
    int unique_id = get_unique_id();
    char *tmp_file_name = ft_strjoin("/tmp/.heredoc_", id_str);
    
    // Ouvrir en écriture
    int fd = open(tmp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    // Lire jusqu'au délimiteur
    read_and_write_heredoc(fd, limiter);
    close(fd);
    
    // Rouvrir en lecture
    fd = open(tmp_file_name, O_RDONLY);
    unlink(tmp_file_name);  // Supprimer après ouverture
    
    return fd;
}
```

---

## Gestion de l'Environnement

### Structure des Variables

```c
typedef struct s_env {
    char            *key;       // HOME, PATH, USER...
    char            *value;     // /home/user, /bin:/usr/bin, user...
    struct s_env    *prev;      // Liste doublement chaînée
    struct s_env    *next;
} t_env;
```

### Initialisation

```c
t_env *get_env(char **envp)
{
    t_env *env = NULL;
    int i = 0;
    
    while (envp[i]) {
        create_env_node(envp[i], &env);
        i++;
    }
    
    return env;
}
```

### Expansion de Variables

```c
char *builtin_expand(char *input, t_env *env)
{
    int dollar_index = check_dollar(input);
    if (dollar_index < 0)
        return NULL;
        
    // Récupérer la valeur de la variable
    char *result = get_expanded_result(&env, &input[dollar_index + 1]);
    return result;
}
```

### Export avec Validation

```c
static int key_name_is_valid(char *key)
{
    if (!key || !key[0])
        return 0;
    
    // Premier caractère : lettre ou underscore
    if (!ft_isalpha(key[0]) && key[0] != '_')
        return 0;
    
    // Caractères suivants : alphanumériques ou underscore
    int i = 1;
    while (key[i]) {
        if (!ft_isalpha(key[i]) && !ft_isdigit(key[i]) && key[i] != '_')
            return 0;
        i++;
    }
    
    return 1;
}
```

---

## Gestion de la Mémoire

### Stratégie Globale

1. **Allocation** : malloc pour toutes les structures
2. **Libération** : Fonctions dédiées pour chaque structure
3. **Ordre** : Libération en ordre inverse de l'allocation
4. **Sécurité** : Vérifications NULL systématiques

### Fonctions de Nettoyage

```c
void clear_shell(t_shell *shell)
{
    if (!shell) return;
    
    if (shell->tokens)
        clear_tokens_lst(&shell->tokens);
    if (shell->ast)
        clear_ast(&shell->ast);
    if (shell->env)
        clear_env_lst(&shell->env);
    clear_lexer_tmp(&shell->lexer);
}
```

### Problème Résolu : Double Free

**Problème initial** :
```c
void clear_cmd(t_cmd *cmd)
{
    if (cmd->name)
        free(cmd->name);        // ❌ Peut causer un double free
    if (cmd->args)
        free_tab_chars(cmd->args);  // ❌ args[0] peut être cmd->name
}
```

**Solution** :
```c
void clear_cmd(t_cmd *cmd)
{
    // Éviter le double free si name == args[0]
    if (cmd->name && (!cmd->args || cmd->name != cmd->args[0])) {
        free(cmd->name);
        cmd->name = NULL;
    }
    
    if (cmd->args) {
        free_tab_chars(cmd->args);
        cmd->args = NULL;
    }
}
```

### File Descriptors

**Initialisation critique** :
```c
static t_ast *init_cmd_node(void)
{
    // ⚠️ IMPORTANT : Initialiser à -1, pas à 0 !
    new_cmd->data.cmd.cmd->fd_in = -1;      // Pas STDIN_FILENO (0)
    new_cmd->data.cmd.cmd->fd_out = -1;     // Pas STDOUT_FILENO (1)
    new_cmd->data.cmd.cmd->fd_heredoc = -1;
}
```

---

## Cas Particuliers et Problèmes Résolus

### 1. Shell qui se termine après une commande

**Problème** : Le shell se fermait après `ls` ou autres commandes.

**Cause** : readline() retournait NULL à cause de file descriptors corrompus.

**Solution** :
- Initialiser les fd à -1 au lieu de 0/1
- Sauvegarder/restaurer STDIN/STDOUT pour les builtins
- Fermer proprement tous les fd après utilisation

### 2. Double Free dans clear_cmd()

**Problème** : Erreur de double free avec `/bin/ls`.

**Cause** : `cmd->name` et `cmd->args[0]` pointaient vers la même zone mémoire.

**Solution** : Vérifier si `cmd->name != cmd->args[0]` avant de libérer.

### 3. Gestion des Guillemets

**Cas particuliers** :
```bash
echo "hello world"          # ✅ hello world
echo 'hello world'          # ✅ hello world
echo hello"world"           # ✅ helloworld (jointure)
echo "hello"world           # ✅ helloworld (jointure)
echo "$HOME"                # ✅ Expansion
echo '$HOME'                # ✅ Littéral $HOME
```

### 4. Validation des Variables d'Environnement

**Règles export** :
- Premier caractère : lettre ou `_`
- Caractères suivants : alphanumériques ou `_`
- Pas de caractères spéciaux

```c
export VAR=value     # ✅ Valid
export _VAR=value    # ✅ Valid
export 1VAR=value    # ❌ Invalid
export VAR-NAME=val  # ❌ Invalid
```

### 5. Heredocs Multiples

```bash
cat << EOF1 << EOF2
content1
EOF1
content2
EOF2
```

**Gestion** : Seul le dernier heredoc est utilisé, les autres sont créés puis fermés.

### 6. Pipes et Redirections

```bash
ls | cat > output.txt       # ✅ Fonctionne
< input.txt cat | grep test # ✅ Fonctionne
```

### 7. Variables d'Environnement Spéciales

```bash
echo $?                     # Code de sortie de la dernière commande
echo $$                     # PID du shell (non implémenté)
echo $HOME                  # Variable d'environnement
```

---

## Codes d'Erreur

### Définitions

```c
#define EXIT_SUCCESS            0    // Succès
#define EXIT_FAILURE            1    // Erreur générale
#define EXIT_SYNTAX_ERROR       2    // Erreur de syntaxe
#define EXIT_CMD_NOT_FOUND    127    // Commande non trouvée
#define EXIT_PERMISSION_DENIED 126   // Permission refusée
#define EXIT_SIGNAL           128    // Terminé par signal
#define EXIT_CTRL_C           130    // Ctrl+C
#define EXIT_CTRL_D           131    // Ctrl+D
```

### Messages d'Erreur

```c
#define SYNTAX_ERROR_PIPE "bash: syntax error near unexpected token '|'"
#define SYNTAX_ERROR_REDIR "bash: syntax error near unexpected token 'newline'"
#define SYNTAX_ERROR_KEY_ENV "bash: export: not a valid identifier"
#define ERROR_CD_MANY_ARGS "bash: cd: too many arguments"
#define ERROR_MISSING_FILE "bash: No such file or directory"
```

### Gestion des Signaux

```c
int get_exit_code(int status)
{
    if (WIFEXITED(status))
        return WEXITSTATUS(status);     // Sortie normale
    else if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status);  // Terminé par signal
    else
        return EXIT_FAILURE;
}
```

---

## Tests et Validation

### Tests de Base

```bash
# Commandes simples
echo hello
ls -la
pwd

# Redirections
echo hello > file.txt
cat < file.txt
echo world >> file.txt

# Pipes
ls | grep .c
cat file.txt | wc -l

# Heredocs
cat << EOF
Hello
World
EOF

# Variables
export VAR=hello
echo $VAR
unset VAR

# Builtins
cd ..
pwd
env
exit
```

### Tests d'Erreur

```bash
# Syntaxe invalide
|               # Erreur: pipe en début
ls |            # Erreur: pipe en fin
< |             # Erreur: redirection invalide
ls > > file     # Erreur: redirections multiples

# Commandes inexistantes
/bin/nonexistent    # Code 127
```

### Tests de Mémoire

```bash
# Valgrind pour détecter les fuites
valgrind --leak-check=full --show-leak-kinds=all ./minishell

# Tests avec des chaînes longues
echo "very long string with many words to test memory allocation"

# Tests avec beaucoup de pipes
cmd1 | cmd2 | cmd3 | cmd4 | cmd5
```

### Tests de Robustesse

```bash
# Guillemets non fermés
echo "hello          # Doit afficher erreur

# Variables inexistantes
echo $NONEXISTENT    # Doit afficher rien

# Fichiers inexistants
cat nonexistent.txt  # Erreur appropriée

# Permissions
echo hello > /root/test  # Permission denied
```

---

## Optimisations et Bonnes Pratiques

### Respect de la Norme 42

1. **25 lignes maximum** par fonction
2. **5 fonctions maximum** par fichier
3. **4 paramètres maximum** par fonction
4. **Pas de variables globales** (sauf g_exit_status pour les signaux)

### Conseils d'Implémentation

1. **Modularité** : Séparer lexer, parser, executor
2. **Structures claires** : AST pour représenter la syntaxe
3. **Gestion d'erreurs** : Codes d'erreur cohérents
4. **Mémoire** : Libération systématique
5. **Tests** : Validation avec bash comme référence

### Extensions Possibles

1. **Signaux** : Gestion de SIGINT, SIGQUIT
2. **Job Control** : Background jobs (&)
3. **Alias** : Raccourcis de commandes
4. **Historique** : Persistance entre sessions
5. **Auto-completion** : Avec readline

---

## Conclusion

Ce minishell implémente les fonctionnalités essentielles d'un shell Unix :

- ✅ **Lexer robuste** avec gestion des guillemets
- ✅ **Parser AST** pour les pipes et redirections
- ✅ **Exécution** fork/exec et builtins
- ✅ **Gestion mémoire** sans fuites
- ✅ **Variables d'environnement** complètes
- ✅ **Gestion d'erreurs** appropriée

### Points Clés pour le Développement

1. **Déboguer pas à pas** : Utiliser display_lexer_results() et display_ast_results()
2. **Tester chaque composant** : Lexer → Parser → Executor
3. **Gérer la mémoire** : Valgrind est votre ami
4. **Valider avec bash** : Comportement de référence
5. **Documenter les cas limites** : Guillemets, variables, erreurs

Cette documentation devrait vous permettre de comprendre, maintenir et étendre le projet minishell efficacement.
