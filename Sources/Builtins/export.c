/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:33:03 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/14 17:18:56 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

static int	ft_copy_env_vars(char **new_envp, char **envp, char *key,
							char *d_new_env)
{
	int	i;
	int	repeated;

	i = 0;
	repeated = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			repeated = 1;
			new_envp[i] = ft_strdup(d_new_env);
		}
		else
			new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			return (-1);
		i++;
	}
	if (repeated)
		return (i);
	else
		return (-i);
}

static char	**ft_add_new_env(char **envp, char *d_new_env, int i)
{
	char	*key;
	char	**new_envp;
	int		len;
	int		copy_result;

	new_envp = ft_init_env_array(envp, d_new_env, &key, &len);
	if (!new_envp)
		return (NULL);
	copy_result = ft_copy_env_vars(new_envp, envp, key, d_new_env);
	if (copy_result < 0)
		return (free(key), ft_utils_free_double_pointer(new_envp), NULL);
	i = copy_result;
	if (copy_result == i)
	{
		new_envp[i] = ft_strdup(d_new_env);
		if (!new_envp[i])
			return (free(key), ft_utils_free_double_pointer(new_envp), NULL);
		i++;
	}
	new_envp[i] = NULL;
	free(key);
	return (new_envp);
}

void	ft_export_add_envp(t_data *data, char *arg, char **new_envp)
{
	if (!arg)
		return ;
	if (!ft_get_env(data, arg))
	{
		new_envp = ft_add_new_env(data->envp, arg, 0);
		if (new_envp)
		{
			ft_utils_free_double_pointer(data->envp);
			data->envp = new_envp;
		}
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
		new_envp = ft_add_new_env(data->envp, arg, 1);
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

void	ft_export(t_data *data)
{
	int		i;
	char	**envp_copy;

	if (!data->cmds->argv[1])
	{
		envp_copy = ft_copy_env(data->envp);
		print_formatted_env(envp_copy);
		ft_utils_free_double_pointer(envp_copy);
		return ;
	}
	i = 1;
	while (data->cmds->argv[i])
	{
		ft_args_export_iterator(data, data->cmds->argv[i]);
		i++;
	}
}
