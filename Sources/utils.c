#include "../Includes/minishell.h"

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

char *split_quotes(char *input, int count, char flag)
{
	char	*single_line;
	int		start;

	start = 1;
	count++;
	single_line = malloc(sizeof(char) * 100);
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

void	ft_clean_quotes(t_data *data)
{
	t_token	*token;

	token = data->token;
	while (data->token->content)
	{
		if (ft_strchr(data->token->content, '\'')
			|| ft_strchr(data->token->content, '\"'))
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