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

char	*ft_get_env(t_data *data, char *env)
{
	int i = 0;

	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], env, ft_strlen(env)))
			return (data->envp[i] + ft_strlen(env) + 1);
		i++;
	}
	return (NULL);
}
