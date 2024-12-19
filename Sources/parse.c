/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 17:02:10 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/19 22:20:20 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	char	**token_table;

	token_table = line_checker(data->input);
	if (!token_table[0])
		token_table[0] = ft_strdup("");
	ft_token_table_maker(token_table, data);
	ft_redirection_check(data);
	ft_check_type(data);
	ft_clean_quotes(data);
}

void	ft_token_table_maker(char **token_table, t_data *data)
{
	int		i;
	t_token	*current_node;
	t_token	*new_node;

	i = 0;
	current_node = NULL;
	while (token_table[i] != NULL)
	{
		new_node = ft_add_node(token_table[i], ft_assign_type(token_table[i]));
		if (i == 0)
		{
			data->head = new_node;
		}
		else
		{
			current_node->next = new_node;
			new_node->prev = current_node;
		}
		current_node = new_node;
		free(token_table[i]);
		i++;
	}
	free(token_table);
	data->token = data->head;
}

char	**ft_get_memory(char *input)
{
	int		count;
	char	**token_table;

	count = ft_character_counter(input, ' ');
	token_table = malloc(sizeof(char *) * (count + 2));
	if (!token_table)
		ft_print_exit("Error: malloc failed\n");
	token_table[count + 1] = NULL;
	return (token_table);
}

char	*ft_assign_type(char *type)
{
	if (type[0] == '|')
		return ("PIPE");
	else if (type[0] == '>' && type[1] == '>')
		return ("APPEND");
	else if (type[0] == '>')
		return ("OUT");
	else if (type[0] == '<' && type[1] == '<')
		return ("HEREDOC");
	else if (type[0] == '<')
		return ("INPUT");
	else if (type[0] == '\'')
		return ("SQUOTE");
	else if (type[0] == '\"')
		return ("DQUOTE");
	else if (type[0] == '$' && type[1])
		return ("ENV");
	else
		return ("ARG");
}
