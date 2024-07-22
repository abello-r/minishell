/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:39:53 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/22 01:58:50 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	ft_redirection_check(char **token_table)
{
	int	a;

	a = 0;
	while (token_table[a])
	{
		if (token_table[a][0] != '\'' && token_table[a][0] != '\"')
		{
			if (ft_redir_conditions_check(token_table[a]))
				ft_print_exit(REDIR);
		}
		a++;
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
	char	**token_table;
	char	*token;
	int		count;
	int 	token_counter;

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
			ft_fill_token_table(token_table, token, token_counter);
			count += ft_strlen(token);
			token_counter++;
		}
		else
		{
			token = get_rest(input, count);
			ft_fill_token_table(token_table, token, token_counter);
			count += ft_strlen(token);
			token_counter++;
		}
		count++;
	}
	printf("token_table. %s", token_table[0]);
		int i = 0;
		while (token_table[i] != NULL) {
			printf("tokentable. %s", token_table[i]);
			i++;
		}
	return (token_table);
}

void	ft_check_type(char **token_table, t_data *data)
{
	int		i;
	int		isbuilt;
	t_token	*temp_token;

	i = 0;
	temp_token = data->token;
	while (token_table[i] != NULL)
	{
		isbuilt = is_builtin(token_table[i], temp_token);
		if (isbuilt == 1)
		{
			temp_token->type = "BUILTIN";
		}
		else
			temp_token->type = ft_assign_type(token_table[i]);
		i++;
		temp_token = temp_token->next;
	}
	temp_token = data->token;
}
