#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	char	**token_table;

	 if (ft_is_empti(data->input) == 1) //redundante
	 	ft_print_exit("Non empty line");
	token_table = line_checker(data->input);
	data->token = malloc(sizeof(t_token));
	if (!data->token)
		ft_print_exit("Error: malloc failed\n");
	tokentablemaker(token_table, data);
	ft_redirection_check(token_table); // mejorar los return
	ft_check_type(token_table, data);
}

t_token	*tokentablemaker(char **token_table, t_data *data)
{
	int		i;
	t_token	*temp_token;

	i = 0;
	temp_token = data->token;
	while (token_table[i] != NULL)
	{
		temp_token->content = token_table[i];
		temp_token->next = malloc(sizeof(t_token));
		if (!temp_token->next)
			ft_print_exit("Error: malloc failed\n");
		temp_token = temp_token->next;
		i++;
	}
	return (data->token);
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
		else if (input[count] == '\'') // esto se acorta en el primer if
		{
			token = split_quotes(input, count, '\'');
			count += (ft_strlen(token));
			ft_fill_token_table(token_table, token);
		}
		else // suficiente?
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
	token_table[i] = malloc(sizeof(char *) * ft_strlen(token));
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
	char	*temp;
	int		count;

	temp = ft_calloc(100, sizeof(char));
	count = 0;
	while (input[i] != ' ' && input[i] != '"'
		&& input[i] != '\'' && input[i] != '\0')
	{
		temp[count] = input[i];
		i++;
		count++;
	}
	return (temp);
}

int	ft_is_empti(char *str)
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

void ft_check_type(char **token_table, t_data *data) // now print type
{
	int		i;
	t_token	*temp_token;

	i = 0;
	temp_token = data->token;
	while (token_table[i] != NULL)
	{
		if (token_table[i][0] == '|')
		{
			// printf("token: %s %s\n", token_table[i], "type: Pipe");
			temp_token->type = "PIPE";
		}
		else if (token_table[i][0] == '>' && token_table[i][1] == '>')
			// printf("token: %s %s\n", token_table[i], "type: append Redirection");
			temp_token->type = "REDIRECTION";
		else if (token_table[i][0] == '>')
			// printf("token: %s %s\n", token_table[i], "type: out");
			temp_token->type = "OUT";
		else if (token_table[i][0] == '<' && token_table[i][1] == '<')
			// printf("token: %s %s\n", token_table[i], "type: heredoc");
			temp_token->type = "HEREDOC";
		else if (token_table[i][0] == '<')
			// printf("token: %s %s\n", token_table[i], "type: input");
			temp_token->type = "INPUT";
		else if (token_table[i][0] == '\'' || token_table[i][0] == '\"')
			// printf("token: %s %s\n", token_table[i], "type: Quote");
			temp_token->type = "QUOTE";
		else if (token_table[i][0] == '$')
			// printf("token: %s %s\n", token_table[i], "type: env");
			temp_token->type = "ENV";
		else
			// printf("token: %s %s\n", token_table[i], "type: ni idea primo");
			temp_token->type = "NIIDEA";
		i++;
		printf("Token: %s Type: %s\n", temp_token->content, temp_token->type);
		temp_token = temp_token->next;
	}
	temp_token = data->token;
}
