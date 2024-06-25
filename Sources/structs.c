#include "../Includes/minishell.h"

void    initializer(char **envp, t_data *data)
{
	fill_data_envp(envp, data);
	fill_path(data);
	data->input = NULL;
    data->token = NULL;
    data->exit = 0;
}
