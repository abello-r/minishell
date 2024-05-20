/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:44:38 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/20 19:20:41 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void ft_env(t_data *data)
{
    int x;
    x = 0;
    if (data->envp == NULL)
        printf("Error: envp is NULL\n");
    else
        while (data->envp[++x] != NULL)
            printf("%s\n", data->envp[x]);
}
