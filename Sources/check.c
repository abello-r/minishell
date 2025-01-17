/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:35:58 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/16 16:44:32 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	ft_redirection_check(t_data *data)
{
	t_token	*current;

	current = data->token;
	while (current != NULL)
	{
		if (current->content && current->content[0] != '\''
			&& current->content[0] != '\"')
		{
			if (ft_redir_conditions_check(current->content))
				ft_print_exit(REDIR);
		}
		current = current->next;
	}
	return (0);
}

int	ft_redir_conditions_check(char *token)
{
	if (token[0] == '>')
	{
		if (token[1] == '>' && token[2] == '>')
			return (1);
		else if (token[1] == '>' && token[2] == '<')
			return (1);
		else if (token[1] == '<')
			return (1);
	}
	else if (token[0] == '<')
	{
		if (token[1] == '<' && token[2] == '<')
			return (1);
		else if (token[1] == '<' && token[2] == '>')
			return (1);
		else if (token[1] == '>')
			return (1);
	}
	return (0);
}

int	ft_pair_quotation_check(t_data *data)
{
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;
	char	*str;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	str = data->input;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	if (in_single_quotes || in_double_quotes)
	{
		printf("%s", NOT_CLOSED_QUOTE);
		return (1);
	}
	return (0);
}

char	**line_checker(char *input)
{
	int		count;
	int		token_counter;
	char	**token_table;
	char	*token;

	count = 0;
	token_counter = 0;
	token_table = ft_get_memory(input);
	while (input[count] != '\0')
	{
		while (input[count] == ' ')
			count++;
		if (input[count] == '\0')
			break ;
		if (input[count] == '\"' || input[count] == '\'')
			token = split_quotes(input, count, input[count]);
		else
			token = get_rest(input, count);
		token_table[token_counter] = ft_fill_token_table(token);
		count += ft_strlen(token);
		free(token);
		token_counter++;
	}
	token_table[token_counter] = NULL;
	return (token_table);
}

void	ft_check_type(t_data *data)
{
	char	*tmp;

	tmp = NULL;
	while (data->token != NULL)
	{
		if (ft_strncmp(data->token->content, "/bin/", 5) == 0)
		{
			tmp = ft_substr(data->token->content, 5, ft_strlen(data->token->content));
			free(data->token->content);
			data->token->content = tmp;
			tmp = NULL;
		}
		if (is_builtin(data->token->content) == 1)
			data->token->type = "BUILTIN";
		else
			data->token->type = ft_assign_type(data->token->content);
		data->token = data->token->next;
	}
	data->token = data->head;
}
