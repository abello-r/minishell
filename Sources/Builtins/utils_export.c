/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:22:14 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/19 19:04:25 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_' )
			return (0);
		str++;
	}
	return (1);
}

char	**ft_copy_env(char **envp)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			ft_utils_free_double_pointer(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
