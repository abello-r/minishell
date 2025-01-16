/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:22:14 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/15 19:36:09 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

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

void	ft_args_export_iterator(t_data *data, char *arg)
{
	char	*var_name;
	char	**new_envp;

	new_envp = NULL;
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		g_status = 1;
		return ;
	}
	if (ft_strchr(arg, '='))
	{
		var_name = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
		new_envp = ft_add_new_env(data->envp, arg);
		if (new_envp)
		{
			ft_utils_free_double_pointer(data->envp);
			data->envp = new_envp;
		}
		free(var_name);
	}
	else
		ft_export_add_envp(data, arg, new_envp);
}
