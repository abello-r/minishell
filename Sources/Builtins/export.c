/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:33:03 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/20 16:41:33 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int g_status;

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

static char **ft_add_new_env(char **envp, char *d_new_env, int i)
{
    int     len;
    int     repeated;
    char    *equals;
    char    *key;
    char    **new_envp;

    if (!envp || !d_new_env)
        return (NULL);

    i = 0;
    repeated = 0;
    equals = ft_strchr(d_new_env, '=');
    if (equals)
        key = ft_substr(d_new_env, 0, equals - d_new_env);
    else
        key = ft_strdup(d_new_env);

    if (!key)
        return (NULL);

    len = ft_envp_len(envp);
    new_envp = malloc(sizeof(char *) * (len + 2));
    if (!new_envp)
    {
        free(key);
        return (NULL);
    }

    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
        {
            repeated = 1;
            new_envp[i] = ft_strdup(d_new_env);
            if (!new_envp[i])
            {
                free(key);
                ft_utils_free_double_pointer(new_envp);
                return (NULL);
            }
        }
        else 
        {
            new_envp[i] = ft_strdup(envp[i]);
            if (!new_envp[i])
            {
                free(key);
                ft_utils_free_double_pointer(new_envp);
                return (NULL);
            }
        }
        i++;
    }
    
    if (repeated == 0)
    {
        new_envp[i] = ft_strdup(d_new_env);
        if (!new_envp[i])
        {
            free(key);
            ft_utils_free_double_pointer(new_envp);
            return (NULL);
        }
        i++;
    }
    new_envp[i] = NULL;
    free(key);
    return (new_envp);
}

void ft_args_export_iterator(t_data *data, char *arg)
{
    char    *equals;
    char    *var_name;
    char    *env_value;
    char    **new_envp;

    if (!is_valid_identifier(arg))
    {
        ft_putstr_fd("minishell: export: `", 2);
        ft_putstr_fd(arg, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        g_status = 1;
        return;
    }
    equals = ft_strchr(arg, '=');
    if (equals)
    {
        var_name = ft_substr(arg, 0, equals - arg);
        new_envp = ft_add_new_env(data->envp, arg, 1);
        if (new_envp)
        {
            ft_utils_free_double_pointer(data->envp);
            data->envp = new_envp;
        }
        free(var_name);
    }
    else
    {
        if (!arg)
            return;
        env_value = ft_get_env(data, arg);
        if (!env_value)
        {
            new_envp = ft_add_new_env(data->envp, arg, 0);
            if (new_envp)
            {
                ft_utils_free_double_pointer(data->envp);
                data->envp = new_envp;
            }
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
