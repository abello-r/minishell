/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:34:15 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/21 22:57:03 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	char	**token_table;

	if (ft_is_empti(data->input) == 1)
		printf("");

	token_table = line_checker(data->input); // OK
	ft_token_table_maker(token_table, data); // OK
	ft_redirection_check(token_table); // OK

	// while (data->token != NULL)
	// {
	// 	printf("%s\n", data->token->content);
	// 	data->token = data->token->next;
	// }

	ft_check_type(token_table, data);
	ft_clean_quotes(data);
	ft_check_dollar(data);
}

void	ft_token_table_maker(char **token_table, t_data *data)
{
	int		i;

	i = 0;
	data->head = data->token;
	while (token_table[i] != NULL)
	{
		data->token->content = malloc(sizeof(char) * (ft_strlen(token_table[i]) + 1));
		data->token->content = token_table[i];
		i++;
		if (token_table[i])
		{
			data->token->next = malloc(sizeof(t_token));
			if (!data->token->next)
				ft_print_exit("Error: malloc failed\n");
			data->token = data->token->next;
		}
	}
	data->token = data->head;
	return ;
}

char    **ft_get_memory(char *input)
{
    char    **token_table;
    int     count;
    int     i;

	i = 0;
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
	else if (type[0] == '$')
		return ("ENV");
	else
		return ("ARG");
}

char	*ft_getenv(char *content, int i, char **envp)
{
	int		x;
	int		y;
	int		start;
	int		end;
	char	*var_name;
	char	*value;
	char	*result;

	start = i + 1;
	end = start;
	while (content[end] && content[end] != ' ')
		end++;
	var_name = ft_substr(content, start, end - start);
	y = 0;
	while (envp[y] != NULL)
	{
		if (ft_strncmp(envp[y], var_name, ft_strlen(var_name)) == 0
			&& envp[y][ft_strlen(var_name)] == '=')
		{
			x = ft_strlen(var_name) + 1;
			value = ft_strdup(&envp[y][x]);
			result = ft_strjoin(value, &content[end]);
			free(var_name);
			free(value);
			return (result);
		}
		y++;
	}
	free(var_name);
	return (ft_strdup(&content[i]));
}
