/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:41:56 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/15 15:19:48 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	char	**split;
	// char	*single_line;
	int 	count;

	count = 0;
	split = ft_modified_split(data->input);
	while (split[count] != NULL)
	{
		printf("split[%d]: %s\n", count, split[count]);
		count++;
	}
}

// void ft_redirect_parse(t_data *data, char c, int start, int count)
// {
// }

void	ft_token_add_back(t_token **token, t_token *new)
{
    t_token *last;
    if (!new)
        return ;
    if (!*token)
    {
        *token = new;
        return ;
    }
    last = ft_token_last(*token);
    last->next = new;
    new->prev = last;
}

t_token *ft_token_last(t_token *token)
{
    if (!token)
        return (NULL);
    while (token->next)
        token = token->next;
    return (token);
}

char	**ft_modified_split(char *input)
{
	char	**split;
	char	*single_line;
	int		count;

	split = ft_allocate_split();
	count = 0;
	while (input[count] != '\0')
	{
		while (input[count] == ' ')
			count++;
		if (input[count] == '\"')
		{
			single_line = split_double_quote(input, count);
			count = (ft_strlen(single_line) - 1);
		}
		// to do: double quotes
		split_char_maker(split, single_line);
	}
	return (split);
}

char **ft_allocate_split()
{
	char	**split;

	split = malloc(sizeof(char *) * 100); //dejar limpio
	if (!split)
		ft_print_exit("Error: malloc failed\n");
	return (split);
}

char *split_double_quote(char *input, int count)
{
	char	*single_line;
	int		start;

	start = 1;
	count++;
	single_line = malloc(sizeof(char) * 100); //dejar limpio
	if (!single_line)
		ft_print_exit("Error: malloc failed\n");
	single_line[0] = '\"';
	while (input[count] != '\"')
	{
		printf("input[%d]: %c\n", count, input[count]);
		single_line[start] = input[count];
		if (input[count - 1] == '\0')
			ft_print_exit("Error: missing double quote\n");
		start++;
		count++;
	}
	single_line[start] = '\"';
	single_line[start + 1] = '\0';
	return (single_line);
}

char	**split_char_maker(char **split, char *splited)
{
	int count;

	count = 0;
	while (split[count] != NULL)
		count++;
	split[count] = splited;
	return (split);
}
