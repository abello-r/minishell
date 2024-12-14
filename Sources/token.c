/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:20:32 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/12 16:20:35 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

char	*ft_fill_token_table(char *token)
{
	int		x;
	char	*tmp_table;

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
