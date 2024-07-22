/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:01:15 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/21 04:01:16 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	ft_fill_token_table(char **token_table, char *token, int token_counter)
{
	int	x;

	x = 0;
	token_table[token_counter] = malloc(sizeof(char *) * ft_strlen(token));
	if (!token_table[token_counter])
		ft_print_exit("Error: malloc failed\n");
	while (token[x] != '\0')
	{
		token_table[token_counter][x] = token[x];
		x++;
	}
	token_table[token_counter][x] = '\0';
	return ;
}

t_token	*ft_token_new(int start, int count, char *input, t_data *data)
{
	t_token	*new;
	char	*content;

	data->token = NULL;
	content = ft_substr(input, start, count - start);
	if (!ft_strcmp(content, "\"\"") || !ft_strcmp(content, "\'\'"))
	{
		free(content);
		return (NULL);
	}
	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		ft_print_exit("Malloc failed\n");
	new->content = content;
	return (new);
}

void	ft_token_add_back(t_token **token, t_token *new)
{
	t_token	*last;

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

t_token	*ft_token_last(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}
