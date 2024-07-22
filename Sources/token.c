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

char	*ft_fill_token_table(char *token)
{
	int		x;
	char 	*tmp_table;

	x = 0;
	tmp_table = malloc(sizeof(char) * ft_strlen(token) + 1);

	if (!tmp_table)
		ft_print_exit("Error: malloc failed\n");
	while (token[x] != '\0')
	{
		tmp_table[x] = token[x];
		x++;
	}
	tmp_table[x] = '\0';
	return (tmp_table);
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
