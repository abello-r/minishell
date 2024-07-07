#include "../Includes/minishell.h"

void	ft_check_dollar(t_data *data)
{
	t_token	*token;
	char	*temp;
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
							data->token->content = ft_substr(data->token->content, 0, i); // hacer
							break ;
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
