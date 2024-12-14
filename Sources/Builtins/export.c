/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:33:03 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/12 16:17:54 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	is_valid_identifier(const char *str)
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

static void	ft_free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
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
			ft_free_array(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

static void	print_formatted_env(char **env)
{
	int		i;
	char	*equals;

	i = 0;
	while (env[i])
	{
		equals = ft_strchr(env[i], '=');
		write(1, "declare -x ", 11);
		if (equals)
		{
			write(1, env[i], equals - env[i]);
			write(1, "=\"", 2);
			write(1, equals + 1, ft_strlen(equals + 1));
			write(1, "\"", 1);
		}
		else
		{
			write(1, env[i], ft_strlen(env[i]));
		}
		write(1, "\n", 1);
		i++;
	}
}

static char	**ft_add_new_env(char **envp, char *d_new_env, int i)
{
	int		repeated;
	char	*key;
	char	**new_envp;

	i = -1;
	repeated = 0;
	key = ft_substr(d_new_env, 0, ft_strchr(d_new_env, '=') - d_new_env);
	new_envp = malloc(sizeof(char *) * (ft_envp_len(envp) + 2));
	ft_check_allocation(new_envp);
	while (envp[++i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			repeated = 1;
			i++;
			free(envp[i]);
			new_envp[i] = ft_strdup(d_new_env);
		}
		else
			new_envp[i] = ft_strdup(envp[i]);
	}
	if (repeated == 0)
		new_envp[i] = ft_strdup(d_new_env);
	new_envp[i + 1] = NULL;
	return (new_envp);
}

void	ft_args_export_iterator(t_data *data, char *arg)
{
	char	*equals;
	char	*var_name;
	char	*env_value;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return ;
	}
	equals = ft_strchr(arg, '=');
	if (equals)
	{
		var_name = ft_substr(arg, 0, equals - arg);
		data->envp = ft_add_new_env(data->envp, arg, 1);
		free(var_name);
	}
	else
	{
		if (!arg)
			return ;
		env_value = ft_get_env(data, arg);
		if (!env_value)
			data->envp = ft_add_new_env(data->envp, arg, 0);
	}
}

void	ft_export(t_data *data)
{
	int		i;
	char	**envp_copy;

	if (!data->cmds->argv[1])
	{
		envp_copy = ft_copy_env(data->envp);
		print_formatted_env(envp_copy);
		ft_free_array(envp_copy);
		return ;
	}
	i = 1;
	while (data->cmds->argv[i])
	{
		ft_args_export_iterator(data, data->cmds->argv[i]);
		i++;
	}
}
