/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:14:57 by abello-r          #+#    #+#             */
/*   Updated: 2024/07/21 00:34:30 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	ft_env(t_data *data)
{
	int	x;

	x = 0;
	if (data->envp == NULL)
		printf("Error: envp is NULL\n");
	else
	{
		while (data->envp[++x] != NULL)
		{
			if (!ft_strchr(data->envp[x], '='))
				continue ;
			printf("%s\n", data->envp[x]);
		}
	}
}
