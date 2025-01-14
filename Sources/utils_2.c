/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:45:33 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/14 15:27:23 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	ft_is_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void	ft_process_export_args(t_data *data)
{
	char	*desired_new_env;

	desired_new_env = NULL;
	while (data->token->next != NULL && data->token->next->content != NULL \
		&& ft_strcmp(data->token->next->type, "ARG") == 0)
	{
		if (data->token->next && data->token->next->content \
			&& ft_strchr(data->token->next->content, '='))
		{
			desired_new_env = ft_strdup(data->token->next->content);
		}
		else if (data->token->next && data->token->next->content)
		{
			desired_new_env = ft_strdup(data->token->next->content);
		}
		ft_args_export_iterator(data, desired_new_env);
		free(desired_new_env);
		data->token = data->token->next;
	}
}