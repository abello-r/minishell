/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:46:20 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/15 18:54:35 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void    data_init(char **envp)
{
    t_data  *data;
    data = malloc(sizeof(t_data));
    data->input = NULL;
    data->envp = parse_envp(envp);
	printf("envp[0]: %s\n", envp[0]);
	printf("envp[1]: %s\n", envp[1]);
	printf("envp[2]: %s\n", envp[2]);
	printf("envp[3]: %s\n", envp[3]);
	printf("envp[4]: %s\n", envp[4]);
	printf("envp[5]: %s\n", envp[5]);
	printf("envp[6]: %s\n", envp[6]);
	printf("envp[7]: %s\n", envp[7]);
    data->path = NULL;
    data->token = NULL;
    data->exit = 0;
}