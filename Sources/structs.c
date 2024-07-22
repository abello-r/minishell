/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:45:23 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/21 03:45:24 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	initializer(char **envp, t_data *data)
{
	fill_data_envp(envp, data);
	fill_path(data);
	data->input = NULL;
	data->token = malloc(sizeof(t_token));
	data->head = data->token;
	if (!data->token)
		ft_print_exit("Error: malloc failed\n");
	data->token->content = NULL;
	data->token->type = NULL;
	data->token->prev = NULL;
	data->token->next = NULL;
	data->exit = 0;
}

size_t	ft_envp_len(char **envp)
{
	size_t	len;

	len = 0;
	while (envp[len] != NULL)
		len++;
	return (len);
}

char	*ft_get_env(t_data *data, char *env)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], env, ft_strlen(env)))
			return (data->envp[i] + ft_strlen(env) + 1);
		i++;
	}
	return (NULL);
}
