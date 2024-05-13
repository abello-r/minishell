/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:51:19 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/13 18:50:21 by briveiro         ###   ########.fr       */
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

# define ERRQ "Error: quotation not closed\n"

typedef struct s_data
{
	char	*input;
}			t_data;

int		ft_is_quotation_closed(char *input, char c);
void	ft_loop(t_data *data);
void	print_exit(char *str);
int		ft_pair_quotation_check(t_data *data);

#endif
