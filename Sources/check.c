/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:35:58 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/18 19:28:39 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	ft_redirection_check(t_data *data)
{
	while (data->token != NULL)
	{
		if (data->token->content[0] != '\'' && data->token->content[0] != '\"')
		{
			if (ft_redir_conditions_check(data->token->content))
				ft_print_exit(REDIR);
		}
		data->token = data->token->next;
	}
	data->token = data->head;
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
	if (ft_character_counter(data->input, '\'') % 2 != 0)
	{
		printf("%s", SIMPLE_QUOTE);
		return (1);
	}
	else if (ft_character_counter(data->input, '\"') % 2 != 0)
	{
		printf("%s", DOUBLE_QUOTE);
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

	token_table = ft_get_memory(input);
	token_counter = 0;
	count = 0;
	while (input[count] != '\0')
	{
		while (input[count] == ' ')
			count++;
		if (input[count] == '\"' || input[count] == '\'')
		{
			token = split_quotes(input, count, input[count]);
			token_table[token_counter] = ft_fill_token_table(token);
			count += ft_strlen(token);
			token_counter++;
		}
		else if (input[count] != 0)
		{
			token = get_rest(input, count);
			token_table[token_counter] = ft_fill_token_table(token);
			count += ft_strlen(token);
			token_counter++;
		}
	}
	token_table[token_counter] = NULL;
	return (token_table);
}

void	ft_check_type(t_data *data)
{
	while (data->token != NULL)
	{
		if (ft_strncmp(data->token->content, "/bin/", 5) == 0)
		{
			data->token->content = ft_substr(data->token->content, 5, \
			ft_strlen(data->token->content));
		}
		if (is_builtin(data->token->content) == 1)
		{
			data->token->type = "BUILTIN";
		}
		else
			data->token->type = ft_assign_type(data->token->content);
		data->token = data->token->next;
	}
	data->token = data->head;
}
