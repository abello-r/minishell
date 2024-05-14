/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:51:19 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/14 15:22:00 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Own includes
# include "../Libft/libft.h"

// System includes
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

# define SIMPLE_QUOTE "Error: quotation not closed"

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
	t_token	*token;
}			t_data;

void	ft_print_exit(char *str);
int		ft_is_quotation_closed(char *input, char c);
void	ft_loop(t_data *data);
int		ft_pair_quotation_check(t_data *data);
void	parser(t_data *data);

#endif
