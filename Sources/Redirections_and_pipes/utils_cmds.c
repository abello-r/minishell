/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:30:06 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 17:31:58 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

t_token	*skip_to_next_command(t_token *tokens)
{
	while (tokens)
	{
		if (ft_strcmp(tokens->type, "PIPE") == 0)
			return (tokens->next);
		tokens = tokens->next;
	}
	return (NULL);
}

int	check_append(t_token *tokens)
{
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "APPEND") == 0)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

char	*extract_output_file(t_token *tokens)
{
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "OUT") == 0 \
			|| ft_strcmp(tokens->type, "APPEND") == 0)
		{
			if (tokens->next && ft_strcmp(tokens->next->type, "ARG") == 0)
				return (ft_strdup(tokens->next->content));
		}
		tokens = tokens->next;
	}
	return (NULL);
}

char	*extract_input_file(t_token *tokens)
{
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "INPUT") == 0)
		{
			if (tokens->next && ft_strcmp(tokens->next->type, "ARG") == 0)
				return (ft_strdup(tokens->next->content));
		}
		tokens = tokens->next;
	}
	return (NULL);
}
