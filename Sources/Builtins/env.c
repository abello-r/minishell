/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:44:38 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/16 17:26:29 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void ft_env(t_data *data)
{
	if (!data->path) {
		printf("No path found\n");
		return ;
	} else {
		printf("%s\n", data->path[0]);
		printf("%s\n", data->path[1]);
		printf("%s\n", data->path[2]);
	}

	/*int x;

	x = 0;
	while (data->path[x] != NULL)
	{
		printf("%s\n", data->path[x]);
		x++;
	}*/
}
