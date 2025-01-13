/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:52:22 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/13 11:32:16 by pausanch         ###   ########.fr       */
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

static char	*ft_replace_content(char *src, char *obj, char *content)
{
    char    *result;
    int     i;
    int     j;
    int     k;
    int     len;

    if (!src || !obj || !content)
        return (NULL);
    
    len = ft_strlen(src) - ft_strlen(obj) + ft_strlen(content) + 1;
    if (!(result = (char *)malloc(sizeof(char) * len)))
        return (NULL);

    i = 0;
    j = 0;
    while (src[i])
    {
        if (ft_strncmp(&src[i], obj, ft_strlen(obj)) == 0)
        {
            k = 0;
            while (content[k])
                result[j++] = content[k++];
            i += ft_strlen(obj);
        }
        else
            result[j++] = src[i++];
    }
    result[j] = '\0';
    
    return (result);
}

static char	*ft_expand_env(t_data *data, char *content)
{
    int i;
    char *org_content;
    char *expanded_value;
    char *result;
    char *tmp;
    
	i= 0;
	org_content = NULL;
	expanded_value = NULL;
	result = NULL;
	tmp = NULL;
    if (!content)
        return (NULL);  
    org_content = ft_strdup(content);
    if (!org_content)
        return (NULL);
    // Special case for $?
    if (content[0] == '$' && content[1] == '?')
    {
        free(org_content);
        char *status_str = ft_itoa(g_status);
        printf("%s", status_str);
        free(status_str);
        return (ft_strdup(""));
    }

    // Handle environment variable expansion
    if (content[0] == '$')
    {
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
    }
    else
    {
        printf("%s", content);
    }
    
    free(org_content);
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

		if (ft_strcmp(data->token->next->type, "ENV") == 0 || ft_strcmp(data->token->next->type, "DQUOTE") == 0)
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
