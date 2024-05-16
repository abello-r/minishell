/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:46:20 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/16 17:33:01 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void    initializer(char **envp, t_data *data)
{
	fill_data_envp(envp, data);
	fill_path(data);
	data->input = NULL;
    data->path = NULL;
    data->token = NULL;
    data->exit = 0;
}
