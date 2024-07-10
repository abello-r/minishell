#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	char	**token_table;

	if (ft_is_empti(data->input) == 1)
		printf("");
	token_table = line_checker(data->input);
	data->token = malloc(sizeof(t_token));
	if (!data->token)
		ft_print_exit("Error: malloc failed\n");
	tokentablemaker(token_table, data);
	ft_redirection_check(token_table);
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

char	**ft_get_memory(void)
{
	char	**token_table;

	token_table = malloc(sizeof(char *) * 100);
	if (!token_table)
		ft_print_exit("Error: malloc failed\n");
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
