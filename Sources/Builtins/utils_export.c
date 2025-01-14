/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:22:14 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 17:19:26 by pausanch         ###   ########.fr       */
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

void	print_formatted_env(char **env)
{
	int		i;
	char	*equals;
	char	*value;

	i = 0;
	while (env[i])
	{
		equals = ft_strchr(env[i], '=');
		write(1, "declare -x ", 11);
		if (equals)
		{
			write(1, env[i], equals - env[i]);
			write(1, "=\"", 2);
			value = equals + 1;
			if (value[0] == '"' && value[ft_strlen(value) - 1] == '"')
				value++ && write(1, value, ft_strlen(value) - 1);
			else
				write(1, value, ft_strlen(value));
			write(1, "\"", 1);
		}
		else
			write(1, env[i], ft_strlen(env[i]));
		write(1, "\n", 1);
		i++;
	}
}

char	**ft_init_env_array(char **envp,
							char *d_new_env, char **key, int *len)
{
	char	*equals;
	char	**new_envp;

	if (!envp || !d_new_env)
		return (NULL);
	equals = ft_strchr(d_new_env, '=');
	if (equals)
		*key = ft_substr(d_new_env, 0, equals - d_new_env);
	else
		*key = ft_strdup(d_new_env);
	if (!*key)
		return (NULL);
	*len = ft_envp_len(envp);
	new_envp = malloc(sizeof(char *) * (*len + 2));
	if (!new_envp)
	{
		free(*key);
		return (NULL);
	}
	return (new_envp);
}
