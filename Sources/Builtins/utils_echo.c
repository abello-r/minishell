/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:57:37 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 16:39:18 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

void	ft_handle_token_print(t_data *data, char *type, int *tokens_count)
{
	char	*expanded_var;

	if (ft_strcmp(type, "ENV") == 0 || ft_strcmp(type, "DQUOTE") == 0)
	{
		expanded_var = ft_expand_env(data, data->token->next->content);
		if (expanded_var)
		{
			printf("%s", expanded_var);
			free(expanded_var);
		}
	}
	else if (*tokens_count >= 1)
		printf("%s ", data->token->next->content);
	else
		printf("%s", data->token->next->content);
	(*tokens_count)--;
}

void	ft_print_echo_args(t_data *data, int dash_flag)
{
	int	tokens_count;

	tokens_count = ft_count_tokens(data) - 1;
	if (dash_flag == 1)
	{
		data->token = data->token->next;
		tokens_count -= 1;
	}
	while (data->token->next != NULL
		&& (ft_strcmp(data->token->next->type, "ARG") == 0
			|| ft_strcmp(data->token->next->type, "DQUOTE") == 0
			|| ft_strcmp(data->token->next->type, "SQUOTE") == 0
			|| ft_strcmp(data->token->next->type, "ENV") == 0))
	{
		ft_handle_token_print(data, data->token->next->type, &tokens_count);
		data->token = data->token->next;
	}
	if (dash_flag == 0)
		printf("\n");
}

char	*handle_special_expansion(char *content)
{
	char	*status_str;

	status_str = NULL;
	if (content[0] == '$' && content[1] == '?')
	{
		status_str = ft_itoa(g_status);
		printf("%s", status_str);
		free(status_str);
		return (ft_strdup(""));
	}
	return (NULL);
}

char	*find_env_value(t_data *data, char *content, char *org_content)
{
	int		i;
	char	*tmp;
	char	*expanded_value;
	char	*result;

	i = 0;
	tmp = ft_substr(content, 1, ft_strlen(content) - 1);
	while (data->envp[i])
	{
		expanded_value = ft_strncmp_turbo(data->envp[i], tmp, ft_strlen(tmp));
		if (expanded_value)
		{
			result = ft_replace_content(org_content, content, expanded_value);
			free(expanded_value);
			free(tmp);
			free(org_content);
			return (result);
		}
		i++;
	}
	free(tmp);
	return (NULL);
}

char	*ft_expand_env(t_data *data, char *content)
{
	char	*org_content;
	char	*special_result;

	if (!content)
		return (NULL);
	special_result = handle_special_expansion(content);
	if (special_result)
		return (special_result);
	org_content = ft_strdup(content);
	if (!org_content)
		return (NULL);
	if (content[0] == '$')
		return (find_env_value(data, content, org_content));
	printf("%s", content);
	free(org_content);
	return (ft_strdup(""));
}
