/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:01:15 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/21 04:01:16 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

char	*ft_fill_token_table(char *token)
{
	int		x;
	char 	*tmp_table;

	x = 0;
	tmp_table = malloc(sizeof(char) * ft_strlen(token) + 1);

	if (!tmp_table)
		ft_print_exit("Error: malloc failed\n");
	while (token[x] != '\0')
	{
		tmp_table[x] = token[x];
		x++;
	}
	tmp_table[x] = '\0';
	return (tmp_table);
}
