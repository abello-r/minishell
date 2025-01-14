/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 17:08:03 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 19:03:20 by pausanch         ###   ########.fr       */
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
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>

# define NOT_CLOSED_QUOTE "[ERROR] Quote not closed\n"
# define REDIR "[ERROR] Redirection not closed\n"

typedef struct s_token
{
	char			*content;
	char			*type;
	struct s_token	*prev;
	struct s_token	*next;
}			t_token;

typedef struct s_cmd
{
	char			**argv;
	char			*input_file;
	char			*output_file;
	int				append;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
	char	*input;
	char	**envp;
	char	**path;
	int		exit;
	t_token	*token;
	t_token	*head;
	int		input_len;
	t_cmd	*cmds;
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
void		cleanup_data(t_data *data);
void		ft_free_commands(t_cmd *cmds);
void		ft_free_tokens(t_token *head);
int			is_builtin(char *content);
int			ft_character_counter(char *str, char c);

// Parser
void		parser(t_data *data);
char		*ft_assign_type(char *type);

// Signals
void		ft_signal_handler(int signum);

// Builtins
void		ft_pwd(void);
void		ft_env(t_data *data);
void		ft_unset(t_data *data);
void		ft_cd(t_data *data);
char 		*validate_directory(char *dir_path, t_data *data);
void		ft_echo(t_data *data);
void		ft_print_echo_args(t_data *data, int dash_flag);
char		*ft_expand_env(t_data *data, char *content);
void		ft_exit(t_data *data);
char		*ft_replace_content(char *s, char *obj, char *c);
char		*ft_strncmp_turbo(const char *s1, const char *s2, size_t n);
void		print_formatted_env(char **env);
char		**ft_init_env_array(char **envp, char *d_new_env, char **key, int *len);


// Export and utils
void		ft_export(t_data *data);
int			is_valid_identifier(const char *str);
char		**ft_copy_env(char **envp);

// Utils
void		initializer(char **envp, t_data *data);
int			ft_character_counter(char *input, char c);
int			ft_pair_quotation_check(t_data *data);
void		ft_print_exit(char *str);
void		ft_redirect_parse(t_data *data);
t_token		*ft_new_token(char *content);
char		**all_the_path(char **path);
char		*get_rest(char *input, int i);
int			ft_is_empty(char *str);
void		ft_token_table_maker(char **token_table, t_data *data);
void		ft_clean_quotes(t_data *data);
void		ft_cpy_clean(t_token *token, int start, int end);
int			is_builtin(char *content);
size_t		ft_envp_len(char **envp);
char		*ft_get_env(t_data *data, char *env);
int			ft_count_tokens(t_data *data);
void		ft_process_export_args(t_data *data);
void		ft_args_export_iterator(t_data *data, char *desired_new_env);
char		*ft_strtolower(char *str);
void		ft_check_allocation(void *mem);
void		clean_quotes_from_env(char *line);
t_token		*ft_add_node(char *content, char *type);
void		ft_utils_free_double_pointer(char **ptr);
void		ft_utils_copy_double_pointer(char **src, char ***dst);

// Split
char		**line_checker(char *input);
char		**ft_get_memory(char *input);
char		*ft_fill_token_table(char *token);
char		*split_quotes(char *input, int count, char flag);

// Environment
void		fill_data_envp(char **envp, t_data *data);
void		fill_path(t_data *data);
void		ft_check_dollar(t_data *data);

// Redirections check

int			ft_redirection_check(t_data *data);
int			ft_redir_conditions_check(char *token);
void		ft_check_type(t_data *data);

// identifiers
void		ft_execute_commands(t_data *data);
t_cmd		*parse_tokens_to_commands(t_token *tokens);
void		print_commands(t_cmd *commands);
void		ft_args_export_iterator(t_data *data, char *arg);

// Heredoc
void		ft_heredoc(t_data *data);
char		*ft_utils_build_command_path(t_data *data, t_cmd *current);
char		**ft_utils_strcpy_turbo(char **src, char *tmp_file);
int			ft_utils_choose_fd_out(t_data *data, char *outfile);


void ft_free_tokens(t_token *head);

// Redirections - utils_cmds
t_token		*skip_to_next_command(t_token *tokens);
int			check_append(t_token *tokens);
char		*extract_output_file(t_token *tokens);
char		*extract_input_file(t_token *tokens);

void		ft_execute_builtin(t_data *data);
char		*find_command_path(t_data *data, char *cmd);
void		handle_command_not_found(char *cmd);
void		setup_redirections(t_cmd *cmd);
void		execute_child(t_data *data, t_cmd *cmd, int *prev_pipe, int *pipe_fd);
void		execute_single_builtin(t_data *data, t_cmd *cmd);
void		ft_check_fd(int fd, t_cmd *cmd);
int			setup_pipes(int *curr_pipe);
void		ft_error_fork(int *curr_pipe);
#endif