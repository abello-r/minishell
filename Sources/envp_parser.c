/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 15:59:44 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/16 17:32:31 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

char *ft_allocate_string(const char *str)
{
	size_t len = ft_strlen(str) + 1;
	char *new_str = malloc(len * sizeof(char));
	if (!new_str)
		return NULL;
	ft_memcpy(new_str, str, len);
	return new_str;
}

void fill_data_envp(char **envp, t_data *data)
{
	int num_env_vars;
	int max_env_vars;
	int i;

	num_env_vars = 0;
	max_env_vars = 100;
	i = 0;
	char **clean_environments = malloc(max_env_vars * sizeof(char *));
	if (!clean_environments)
		return;
	while (envp[num_env_vars] != NULL)
		num_env_vars++;
	while (envp[i] != NULL)
	{
		clean_environments[i] = ft_allocate_string(envp[i]);
		if (clean_environments[i] == NULL)
			while (i-- > 0)
				free(clean_environments[i - 1]);
		i++;
	}
	clean_environments[num_env_vars] = NULL;
	data->envp = clean_environments;
}

void fill_path(t_data *data)
{
	int i;
	char **path_blocks;

	i = 0;

	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PATH=", 5) == 0) {
			path_blocks = ft_split(&data->envp[i][5], ':');
		}
		i++;
	}
}
