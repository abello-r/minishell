/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:41:56 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/14 18:41:57 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	int	count;

	count = 0;
	while (data->input[count] == ' ')
		count++;
	if (ft_strchr("'\"''\''", data->input[count]))
	{
		
	}
}

// void	check_all_quotes(t_data *data)
// {
	
// }