/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:46:20 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/14 20:47:01 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void    data_init(char **envp)
{
    t_data  *data;
    data = malloc(sizeof(t_data));
    data->input = NULL;
    data->envp = envp;
    data->path = NULL;
    data->token = NULL;
    data->exit = 0;
}