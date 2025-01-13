/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:45:33 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/13 12:30:21 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

char	*get_rest(char *input, int i)
{
	int		count;
	int		start;
	char	*temp;

	count = 0;
	start = i;
	while (input[i] != ' ' && input[i] != '\0')
	{
		i++;
		count++;
	}
	temp = ft_substr(input, start, count);
	temp[count] = '\0';
	return (temp);
}

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

char	*split_quotes(char *input, int count, char flag)
{
	char	*single_line;
	int		start;

	start = 1;
	count++;
	single_line = malloc(sizeof(char) * (ft_strlen(input) + 1));
	single_line[0] = flag;
	if (!single_line)
		ft_print_exit("Error: malloc failed\n");
	while (input[count] != flag && input[count] != '\0')
	{
		single_line[start] = input[count];
		if (input[count - 1] == '\0')
			ft_print_exit("Error: missing double quote\n");
		start++;
		count++;
	}
	single_line[start] = flag;
	single_line[start + 1] = '\0';
	return (single_line);
}

char *ft_remove_quotes(char *str)
{
    char *result;
    size_t len;

    if (!str)
        return (NULL);
    len = strlen(str);
    if (len < 2 || ((str[0] != '\'' && str[0] != '\"') || str[len - 1] != str[0]))
        return (strdup(str));
    result = malloc(len - 1);
    if (!result)
        return (NULL);
    strncpy(result, str + 1, len - 2);
    result[len - 2] = '\0';
    return (result);
}

void ft_clean_quotes(t_data *data)
{
    t_token *current;
	char *new_content;

    current = data->head;
    while (current)
    {
        if (current->type && (strcmp(current->type, "SQUOTE") == 0 || strcmp(current->type, "DQUOTE") == 0))
        {
            new_content = ft_remove_quotes(current->content);
            if (new_content)
            {
                free(current->content); // Libera el contenido anterior
                current->content = new_content; // Reemplaza por el nuevo contenido sin comillas
            }
        }
        current = current->next;
    }
}

void	ft_cpy_clean(t_token *token, int start, int end)
{
	int		i;
	char	*temp;

	i = 0;
	temp = ft_calloc(100, sizeof(char));
	if (!temp)
		ft_print_exit("Error: malloc failed\n");
	if (token->content[0] == '\'')
		token->type = "SQUOTE";
	else if (token->content[0] == '\"')
		token->type = "DQUOTE";
	while (token->content[start] != '\0' && start < end)
	{
		temp[i] = token->content[start];
		i++;
		start++;
	}
	temp[i] = '\0';
	token->content = temp;
}
