/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:51:19 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/14 17:16:56 by abello-r         ###   ########.fr       */
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
	char			*content;
	char			*type;
	struct	s_token	*prev;
	struct	s_token	*next;
}			t_token;

typedef struct s_data
{
	char	*input;
	t_token	*token;
}			t_data;

int		ft_character_counter(char *input, char c);
int		ft_loop(t_data *data);
int		ft_pair_quotation_check(t_data *data);
void	ft_signal_handler(int signum);
void	ft_print_exit(char *str);
void	parser(t_data *data);

#endif
