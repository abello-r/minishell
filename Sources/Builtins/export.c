/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:33:03 by abello-r          #+#    #+#             */
/*   Updated: 2024/07/10 02:10:22 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

char	**ft_add_new_env(char **envp, char *d_new_env, int i)
{
	int		repeated;
	char	*key;
	char	**new_envp;

	i = -1;
	repeated = 0;
	key = ft_substr(d_new_env, 0, ft_strchr(d_new_env, '=') - d_new_env);
	new_envp = malloc(sizeof(char *) * (ft_envp_len(envp) + 2));
	if (!new_envp)
		ft_print_exit("Error: malloc failed\n");
	while (envp[++i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			repeated = 1 && i + 1;
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

void	ft_add_quotes(int i, char **envp_copy)
{
	int	j;
	int	k;

	j = 0;
	while (envp_copy[i][j] != '\0')
	{
		if (envp_copy[i][j] == '=')
		{
			k = ft_strlen(envp_copy[i]);
			while (k > j)
			{
				envp_copy[i][k + 1] = envp_copy[i][k];
				k--;
			}
			envp_copy[i][j + 1] = '"';
			break ;
		}
		j++;
	}
}

char	**ft_copy_env(char **envp)
{
	int		i;
	int		envp_len;
	char	**envp_copy;

	envp_len = ft_envp_len(envp);
	envp_copy = malloc(sizeof(char *) * (envp_len + 1));
	if (!envp_copy)
		ft_print_exit("Error: malloc failed\n");
	i = 0;
	while (envp[i] != NULL)
	{
		envp_copy[i] = malloc(ft_strlen(envp[i]) + 14);
		if (!envp_copy[i])
			ft_print_exit("Error: malloc failed\n");
		ft_strcpy(envp_copy[i], "declare -x ");
		ft_strlcat(envp_copy[i], envp[i], ft_strlen(envp[i]) + 12);
		ft_add_quotes(i, envp_copy);
		if (ft_strchr(envp_copy[i], '='))
			ft_strlcat(envp_copy[i], "\"", ft_strlen(envp_copy[i]) + 2);
		i++;
	}
	envp_copy[envp_len] = NULL;
	return (envp_copy);
}

void	ft_args_iterator(t_data *data, char *desired_new_env)
{
	desired_new_env = data->token->next->content;
	if (ft_isalpha(desired_new_env[0]) == 0)
	{
		printf("minishell: export: `%s': \
			not a valid identifier\n", desired_new_env);
		return ;
	}
	else
		data->envp = ft_add_new_env(data->envp, desired_new_env, 0);
}

void	ft_export(t_data *data)
{
	int		i;
	char	*desired_new_env;
	char	**envp_copy;

	i = 0;
	desired_new_env = NULL;
	envp_copy = ft_copy_env(data->envp);
	if (data->token->next->content == NULL)
	{
		while (envp_copy[++i] != NULL)
			printf("%s\n", envp_copy[i]);
		return ;
	}
	else
	{
		while (data->token->next->content != NULL && \
			ft_strcmp(data->token->next->type, "ARG") == 0)
		{
			ft_args_iterator(data, desired_new_env);
			data->token = data->token->next;
		}
	}
}
