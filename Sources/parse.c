#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	char	**token_table;

	token_table = line_checker(data->input);
	printf("Ignored line %s\n", token_table[0]);
}

char	**line_checker(char *input)
{
	char	**token_table;
	char	*token;
	int		count;

	token_table = ft_get_memory();
	count = 0;
	while (input[count] != '\0')
	{
		while (input[count] == ' ')
			count++;
		if (input[count] == '\"')
		{
			token = split_quotes(input, count, '\"');
			count += (ft_strlen(token));
			ft_fill_token_table(token_table, token);
		}
		else if (input[count] == '\'')
		{
			token = split_quotes(input, count, '\'');
			count += (ft_strlen(token));
			ft_fill_token_table(token_table, token);
		}
		else
		{
			token = get_rest(input, count);
			count += (ft_strlen(token));
			ft_fill_token_table(token_table, token);
		}
		count++;
	}
	count = 0;
	while (token_table[count] != NULL)
	{
		printf("Token %d: %s\n", count, token_table[count]);
		count++;
	}
	return (token_table);
}

char **ft_get_memory(void)
{
	char	**token_table;

	token_table = malloc(sizeof(char *) * 100); //dejar limpio
	if (!token_table)
		ft_print_exit("Error: malloc failed\n");
	return (token_table);
}

char	**ft_fill_token_table(char **token_table, char *token)
{
	int i;
	int x;

	i = 0;
	x = 0;

	while (token_table[i] != NULL)
		i++;
	token_table[i] = malloc(sizeof(char *) *ft_strlen(token));
	if (!token_table[i])
		ft_print_exit("Error: malloc failed\n");
	while (token[x] != '\0')
	{
		token_table[i][x] = token[x];
		x++;
	}
	token_table[i][x] = '\0';
	return (token_table);
}

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

char *split_quotes(char *input, int count, char flag)
{
	char	*single_line;
	int		start;

	start = 1;
	count++;
	single_line = malloc(sizeof(char) * 100); //dejar limpio
	if (!single_line)
		ft_print_exit("Error: malloc failed\n");
	single_line[0] = flag;
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

char	*get_rest(char *input, int i)
{
	char	*aux;
	int		j;

	aux = ft_calloc(100, sizeof(char));
	j = 0;
	while (input[i] != ' ' && input[i] != '"'
		&& input[i] != '\'' && input[i] != '\0')
	{
		aux[j] = input[i];
		i++;
		j++;
	}
	return (aux);
}
