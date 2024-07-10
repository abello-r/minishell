#include "../Includes/minishell.h"

void    initializer(char **envp, t_data *data)
{
	fill_data_envp(envp, data);
	fill_path(data);
	data->input = NULL;
    data->token = NULL;
    data->exit = 0;
}
size_t	ft_envp_len(char **envp)
{
	size_t	len;

	len = 0;
	while (envp[len] != NULL)
		len++;
	return (len);
}
