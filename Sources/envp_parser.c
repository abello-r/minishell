/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 15:59:44 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/16 16:20:54 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

char *ft_allocate_string(const char *str) {
    size_t len = ft_strlen(str) + 1;
    char *new_str = malloc(len * sizeof(char));
    if (!new_str)
        return NULL;
    ft_memcpy(new_str, str, len);
    return new_str;
}

void fill_data_envp(char **envp, t_data *data) {
    int num_env_vars;
    int max_env_vars;
	int i;

	num_env_vars = 0;
	max_env_vars = 100;
	i = 0;
    char **clean_environments = malloc(max_env_vars * sizeof(char *));
    if (!clean_environments) 
		return;
    while (envp[num_env_vars] != NULL) {
        num_env_vars++;
    }
	while (envp[i] != NULL) {
	    clean_environments[i] = ft_allocate_string(envp[i]);
	    if (clean_environments[i] == NULL) {
			while (i > 0) {
				free(clean_environments[i - 1]);
				i--;
			}
			return;
	    }
	    i++;
	}
    clean_environments[num_env_vars] = NULL;
	data->envp = clean_environments;
}

void fill_path(t_data *data)
{
	char    **spl;
	int     count;
	char    *tmp;
	count = 0;
	while (data->envp[count])
	{
		if (data->envp[count][0] == 'P' && data->envp[count][1] == 'A'
			&& data->envp[count][2] == 'T')
			break ;
		count++;
	}
	spl = ft_split(&data->envp[count][5], ':');
	count = -1;
	while (spl[++count])
	{
		tmp = ft_strjoin(spl[count], "/");
		free(spl[count]);
		spl[count] = tmp;
	}
	data->path = spl;
}
