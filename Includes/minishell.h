/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:51:19 by abello-r          #+#    #+#             */
/*   Updated: 2024/06/07 05:16:26 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Own includes
# include "../Libft/libft.h"

// System includes
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define SIMPLE_QUOTE "[ERROR] Simple quote not closed\n"
# define DOUBLE_QUOTE "[ERROR] Double quote not closed\n"
# define REDIR "[ERROR] Redirection not closed\n"

typedef struct s_token
{
    char            *content;
    char            *type;
    struct  s_token *prev;
    struct  s_token *next;
}           t_token;

typedef struct s_data
{
    char    *input;
    char    **envp;
	char   	**path;
    int     exit;
    t_token *token;
	int		input_len;
    // s_enum   *env;
}           t_data;

// Main
int		ft_loop(t_data *data);

// Parser
void	parser(t_data *data);

// Signals
void	ft_signal_handler(int signum);

// Builtins
void	ft_pwd(void);
void	ft_env(t_data *data);
void    ft_unset(t_data *data, char *str);
void    ft_export(t_data *data);

// Utils
void   		initializer(char **envp, t_data *data);
int			ft_character_counter(char *input, char c);
int			ft_pair_quotation_check(t_data *data);
void		ft_print_exit(char *str);
void   		ft_redirect_parse(t_data *data); //doing
t_token		*ft_new_token(char *content); //making
char   		**all_the_path(char **path);
t_token		*ft_token_last(t_token *token);
char		*get_rest(char *input, int i);
int			ft_is_empti(char *str);
t_token		*tokentablemaker(char **token_table, t_data *data);

// Split
char		**line_checker(char *input);
char		**ft_get_memory(void);
char		**ft_fill_token_table(char **token_table, char *token);
char		*split_quotes(char *input, int count, char flag);

// Environment
void		fill_data_envp(char **envp, t_data *data);
void		fill_path(t_data *data);

// Redirections check

int			ft_redirection_check(char **token_table);
int			ft_redir_conditions_check(char *token);

// identifiers
void		ft_check_type(char **token_table, t_data *data);


#endif