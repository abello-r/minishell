/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:44:38 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/15 19:20:59 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void ft_env(t_data *data)
{
	int x = 0;
	int y = 0;

	while (data->envp[x])
	{
		while (data->envp[x][y])
		{
			write(1, &data->envp[x][y], 1);
			y++;
		}
		write(1, "\n", 1);
		y = 0;
		x++;
	}

}