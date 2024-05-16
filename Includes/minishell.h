/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:51:19 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/16 16:21:43 by abello-r         ###   ########.fr       */
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

// Utils
void   		initializer(char **envp, t_data *data);
int			ft_character_counter(char *input, char c);
int			ft_pair_quotation_check(t_data *data);
void		ft_print_exit(char *str);
void   		ft_redirect_parse(t_data *data); //doing
t_token		*ft_new_token(char *content); //making
char   		**all_the_path(char **path);
t_token		*ft_token_last(t_token *token);

// Split
char		**line_checker(char *input);
char		**ft_get_memory(void);
char		**ft_fill_token_table(char **token_table, char *token);
char		*split_double_quote(char *input, int count);

// Environment
void		fill_data_envp(char **envp, t_data *data);
void		fill_path(t_data *data);
#endif
