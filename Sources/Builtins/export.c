/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:33:03 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/15 19:46:23 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

static char	**ft_copy_env_values(char **envp, char *d_new_env, char *key,
								int *repeated)
{
	int		i;
	int		len;
	char	**new_envp;

	len = ft_envp_len(envp);
	new_envp = malloc(sizeof(char *) * (len + 2));
	if (!new_envp)
		return (NULL);
	i = 0;
	*repeated = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			*repeated = 1;
			new_envp[i] = ft_strdup(d_new_env);
		}
		else
			new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			return (ft_utils_free_double_pointer(new_envp), NULL);
		i++;
	}
	return (new_envp);
}

static char	**ft_handle_env_addition(char **new_envp, char **envp,
									char *d_new_env, int repeated)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	if (repeated == 0)
	{
		new_envp[i] = ft_strdup(d_new_env);
		if (!new_envp[i])
		{
			ft_utils_free_double_pointer(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

char	**ft_add_new_env(char **envp, char *d_new_env)
{
	char	**new_envp;
	char	*equals;
	char	*key;
	int		repeated;

	if (!envp || !d_new_env)
		return (NULL);
	equals = ft_strchr(d_new_env, '=');
	if (equals)
		key = ft_substr(d_new_env, 0, equals - d_new_env);
	else
		key = ft_strdup(d_new_env);
	if (!key)
		return (NULL);
	new_envp = ft_copy_env_values(envp, d_new_env, key, &repeated);
	if (!new_envp)
		return (free(key), NULL);
	new_envp = ft_handle_env_addition(new_envp, envp, d_new_env, repeated);
	if (!new_envp)
		return (free(key), NULL);
	return (free(key), new_envp);
}

void	ft_export_add_envp(t_data *data, char *arg, char **new_envp)
{
	if (!arg)
		return ;
	if (!ft_get_env(data, arg))
	{
		new_envp = ft_add_new_env(data->envp, arg);
		if (new_envp)
		{
			ft_utils_free_double_pointer(data->envp);
			data->envp = new_envp;
		}
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
