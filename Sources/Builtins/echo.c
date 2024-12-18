/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:52:22 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/18 17:03:43 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int g_status;

char	*ft_strncmp_turbo(const char *s1, const char *s2, size_t n)
{
	size_t	c;

	c = 0;
	if (n == 0)
		return (NULL);
	while (s1[c] && s2[c] && s1[c] == s2[c] && s1[c] != '=' && s2[c] != '=')
		c++;
	if (s2[c] == '\0' && s1[c] == '=')
		return (ft_strdup(s1 + c + 1));
	return (NULL);
}

static char	*ft_expand_env(t_data *data, char *env)
{
	int		i;
	char	*expanded_value;

	i = 0;
	
	if (env[0] == '$' && env[1] == '?')
	{
		printf("%d", g_status);
		return (ft_strdup(""));
	}
	else if (env[0] == '$')
		env = env + 1;
	while (data->envp[i])
	{
		expanded_value = ft_strncmp_turbo(data->envp[i], env, ft_strlen(env));
		if (expanded_value != NULL)
			return (expanded_value);
		i++;
	}
	return (ft_strdup(""));
}

static void	ft_print_echo_args(t_data *data, int dash_flag)
{
	int		tokens_count;
	char	*expanded_var;

	tokens_count = ft_count_tokens(data) - 1;
	if (dash_flag == 1)
	{
		data->token = data->token->next;
		tokens_count -= 1;
	}
	while (data->token->next != NULL && \
		(ft_strcmp(data->token->next->type, "ARG") == 0 \
			|| ft_strcmp(data->token->next->type, "DQUOTE") == 0 \
			|| ft_strcmp(data->token->next->type, "SQUOTE") == 0 \
			|| ft_strcmp(data->token->next->type, "ENV") == 0))
	{
		if (ft_strcmp(data->token->next->type, "ENV") == 0)
		{
			expanded_var = ft_expand_env(data, data->token->next->content);
			if (expanded_var)
			{
				printf("%s", expanded_var);
				free(expanded_var);
			}
		}
		else if (tokens_count >= 1)
			printf("%s ", data->token->next->content);
		else
			printf("%s", data->token->next->content);
		data->token = data->token->next;
		tokens_count--;
	}
	if (dash_flag == 0)
		printf("\n");
}

static int	check_dash_flag(t_data *data)
{
	int	i;

	i = 0;
	if (ft_strcmp(data->token->next->type, "ARG") != 0)
		return (0);
	while (data->token->next->content[i] == '-')
	{
		if (data->token->next->content[i] == '-' && \
			data->token->next->content[i + 1] == 'n')
			return (1);
		i++;
	}
	return (0);
}

void	ft_echo(t_data *data)
{
	data->token = data->head;
	if (data->token->next == NULL)
		printf("\n");
	else
	{
		if (check_dash_flag(data))
			ft_print_echo_args(data, 1);
		else
			ft_print_echo_args(data, 0);
	}
}
