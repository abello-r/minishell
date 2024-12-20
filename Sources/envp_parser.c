/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:42:34 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/20 17:29:12 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

char	*ft_allocate_string(const char *str)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(str) + 1;
	new_str = malloc(len * sizeof(char));
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, str, len);
	return (new_str);
}

void	fill_data_envp(char **envp, t_data *data)
{
	int		i;
	int		num_env_vars;
	int		max_env_vars;
	char	**clean_environments;

	num_env_vars = 0;
	max_env_vars = 100;
	i = 0;
	clean_environments = malloc(max_env_vars * sizeof(char *));
	if (!clean_environments)
		return ;
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
	ft_utils_copy_double_pointer(clean_environments, &data->envp);
	ft_utils_free_double_pointer(clean_environments);
}

/* void	fill_path(t_data *data)
{
	int		i;
	char	**path_blocks;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PATH=", 5) == 0)
			path_blocks = ft_split(&data->envp[i][5], ':');
		i++;
	}
	data->path = path_blocks;
} */

void fill_path(t_data *data)
{
    int     i;
    char    **path_blocks;

    i = 0;
    path_blocks = NULL;
    while (data->envp[i])
    {
        if (ft_strncmp(data->envp[i], "PATH=", 5) == 0)
        {
            path_blocks = ft_split(&data->envp[i][5], ':');
            if (!path_blocks)
                ft_print_exit("Error: malloc failed\n");
            break;
        }
        i++;
    }
    data->path = path_blocks;
}

