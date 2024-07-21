/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:46:27 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/21 12:55:21 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Own includes
# include "../Libft/libft.h"

// System includes
# include	<stdio.h>
# include	<unistd.h>
# include	<stdlib.h>
# include	<signal.h>
# include	<readline/readline.h>
# include	<readline/history.h>
# include	<sys/wait.h>

# define SIMPLE_QUOTE "[ERROR] Simple quote not closed\n"
# define DOUBLE_QUOTE "[ERROR] Double quote not closed\n"
# define REDIR "[ERROR] Redirection not closed\n"

extern int g_status;

typedef struct s_token
{
	char			*content;
	char			*type;
	struct s_token	*prev;
	struct s_token	*next;
}			t_token;

typedef struct s_data
{
	char	*input;
	char	**envp;
	char	**path;
	int		exit;
	t_token	*token;
	int		input_len;
}			t_data;

typedef enum s_enum
{
	ENV,
	PWD,
	CD,
	EXPORT,
	UNSET,
	EXIT,
	ECHO,
	UNKNOWN
}	t_enum;

// Main
int			ft_loop(t_data *data);

// Parser
void		parser(t_data *data);

// Signals
void		ft_signal_handler(int signum);

// Builtins
void		ft_pwd(void);
void		ft_env(t_data *data);
void		ft_unset(t_data *data);
void		ft_export(t_data *data);
void		ft_cd(t_data *data);
void		ft_echo(t_data *data);
void		ft_exit(t_data *data);

// Utils
void		initializer(char **envp, t_data *data);
int			ft_character_counter(char *input, char c);
int			ft_pair_quotation_check(t_data *data);
void		ft_print_exit(char *str);
void		ft_redirect_parse(t_data *data); //doing
t_token		*ft_new_token(char *content); //making
char		**all_the_path(char **path);
t_token		*ft_token_last(t_token *token);
char		*get_rest(char *input, int i);
int			ft_is_empti(char *str);
t_token		*tokentablemaker(char **token_table, t_data *data);
void		ft_clean_quotes(t_data *data);
void		ft_cpy_clean(t_token *token, int start, int end);
int			is_builtin(char *token_table, t_token *token);
size_t		ft_envp_len(char **envp);
char		*ft_get_env(t_data *data, char *env);
int			ft_count_tokens(t_data *data);
void		ft_check_cmd_on_path(t_data *data);
void		ft_process_export_args(t_data *data);
void		ft_args_export_iterator(t_data *data, char *desired_new_env);
char		*ft_strtolower(char *str);
void		ft_check_allocation(void *mem);

// Split
char		**line_checker(char *input);
char		**ft_get_memory(void);
char		**ft_fill_token_table(char **token_table, char *token);
char		*split_quotes(char *input, int count, char flag);

// Environment
void		fill_data_envp(char **envp, t_data *data);
void		fill_path(t_data *data);
void		ft_check_dollar(t_data *data);
char		*ft_getenv(char *content, int i, char **envp);

// Redirections check

int			ft_redirection_check(char **token_table);
int			ft_redir_conditions_check(char *token);

// identifiers
void		ft_check_type(char **token_table, t_data *data);
char		*ft_assign_type(char *type);

#endif