/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:52:01 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/18 14:07:20 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static char	*ft_get_key(char *token)
{
	int		i;
	char	*key;

	i = 0;
	while (token[i] != '=')
		i++;
	key = ft_substr(token, 0, i);
	return (key);
}

static char	**ft_create_copy_without_key(t_data *data, char *key)
{
	char	**copy;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (data->envp[i] != NULL)
		i++;
	copy = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(ft_get_key(data->envp[i]), key, ft_strlen(key)) != 0)
		{
			copy[j] = ft_strdup(data->envp[i]);
			j++;
		}
		i++;
	}
	copy[j] = NULL;
	return (copy);
}

static int	ft_str_include_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

static void	ft_args_iterator(t_data *data)
{
	int		i;
	char	*key;
	char	**copy;

	i = 0;
	while (data->envp[i] != NULL)
	{
		key = ft_get_key(data->envp[i]);
		if (ft_strcmp(key, data->token->next->content) == 0)
		{
			copy = ft_create_copy_without_key(data, key);
			free(data->envp);
			free(key);
			data->envp = copy;
			break ;
		}
		i++;
	}
}

void	ft_unset(t_data *data)
{
	while (data->token->next != NULL && \
			ft_strcmp(data->token->next->type, "ARG") == 0)
	{
		if (data->envp == NULL || data->token->next->content == NULL)
			return ;
		if (ft_str_include_equal(data->token->next->content) == 1)
		{
			printf("minishell: unset: `%s': not a valid identifier\n",
				data->token->next->content);
			return ;
		}
		ft_args_iterator(data);
		data->token = data->token->next;
	}
}
