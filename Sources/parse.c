#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	char	**token_table;

	if (ft_is_empti(data->input) == 1)
		ft_print_exit("Non empty line");
	token_table = line_checker(data->input);
	data->token = malloc(sizeof(t_token));
	if (!data->token)
		ft_print_exit("Error: malloc failed\n");
	// comprobar si es builtin
	tokentablemaker(token_table, data);
	ft_redirection_check(token_table); // mejorar los return
	ft_check_type(token_table, data);
	ft_clean_quotes(data);
	ft_check_dollar(data);
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
		if (input[count] == '\"' || input[count] == '\'')
		{
			token = split_quotes(input, count, input[count]);
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
	return (token_table);
}

char	**ft_get_memory(void)
{
	char	**token_table;

	token_table = malloc(sizeof(char *) * 100); //dejar limpio
	if (!token_table)
		ft_print_exit("Error: malloc failed\n");
	return (token_table);
}

char	**ft_fill_token_table(char **token_table, char *token)
{
	int	i;
	int	x;

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
	single_line[0] = flag;
	if (!single_line)
		ft_print_exit("Error: malloc failed\n");
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
	printf("Single line: %s\n", single_line);
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
		is_builtin(token_table[i], temp_token);
		temp_token->type = ft_assign_type(token_table[i]);
		i++;
		temp_token = temp_token->next;
	}
	temp_token = data->token;
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
void	ft_check_dollar(t_data *data) // hacer, no está terminado
{// si el valor no existe?
	t_token	*token;
	char 	*temp;
	int		i;

	token = data->token;
	temp = ft_calloc(100, sizeof(char));
	while (data->token->content)
	{
		i = 0;
		if (ft_strncmp(data->token->type, "SQU", 3))
		{
			while (data->token->content[i])
			{
				if (data->token->content[i] == '$'
					&& data->token->content[i + 1] != '\0'
					&& data->token->content[i + 1] != ' ')
				{
					if (data->token->content[i + 1] == '?')
					{
							data->token->content = ft_substr(data->token->content, 0, i);
							break;
					}
					else
						data->token->content = ft_getenv(data->token->content, i, data->envp);
						data->token->content = ft_strjoin(temp, data->token->content);
						free(temp);
						break;
				}
				else
					temp[i] = data->token->content[i];
				i++;
			}
		}
		printf("Token: %s   Type; %s\n", data->token->content, data->token->type);
		data->token = data->token->next;
	}
	data->token = token;
}

char	*ft_getenv(char *content, int i, char **envp)
{
	int		x;
	int		y;
	char	*temp;

	x = i + 1;
	y = 0;
	temp = ft_calloc(100, sizeof(char));
	while (content[x] != '\0' && content[x] != ' ')
	{
		temp[y] = content[x];
		x++;
		y++;
	}
	temp[y] = '\0';
	y = 0;
	while (envp[y] != NULL)
	{
		if (ft_strncmp(envp[y], temp, ft_strlen(temp)) == 0)
		{
			x = 0;
			while (envp[y][x] != '=')
				x++;
			x++;
			i = 0;
			while (envp[y][x] != '\0')
			{
				content[i] = envp[y][x];
				x++;
				i++;
			}
			content[i] = '\0';
			return (content);
		}
		y++;
	}
	return (content);
}

void	ft_clean_quotes(t_data *data)
{
	t_token	*token;

	token = data->token;
	while (data->token->content)
	{
		if (ft_strchr(data->token->content, '\'') || ft_strchr(data->token->content, '\"'))
		{
			ft_cpy_clean(data->token, 1, ft_strlen(data->token->content) - 1);
		}
		data->token = data->token->next;
	}
	data->token = token;
}

void	ft_cpy_clean(t_token *token, int start, int end)
{
	int		i;
	char	*temp;

	i = 0;
	temp = ft_calloc(100, sizeof(char));
	if (!temp)
		ft_print_exit("Error: malloc failed\n");
	if (token->content[0] == '\'')
		token->type = "SQUOTE";
	else if (token->content[0] == '\"')
		token->type = "DQUOTE";
	while (token->content[start] != '\0' && start < end)
	{
		temp[i] = token->content[start];
		i++;
		start++;
	}
	temp[i] = '\0';
	token->content = temp;
}