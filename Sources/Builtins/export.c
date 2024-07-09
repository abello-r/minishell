#include "../../Includes/minishell.h"

// TODO:

// 1. Agregar la variable nueva si no tiene = a export pero no a envp
// 2. Norminette

size_t ft_envp_len(char **envp)
{
	size_t len = 0;
	while (envp[len] != NULL)
	{
		len++;
	}
	return len;
}

char **ft_add_new_env(char **envp, char *desired_new_env)
{
	int i;
	int envp_len;
	char **new_envp;

	envp_len = ft_envp_len(envp);
	new_envp = malloc(sizeof(char *) * (envp_len + 2));

	if (ft_strchr(desired_new_env, '=') == NULL)
		return envp;

	if (!new_envp)
		ft_print_exit("Error: malloc failed\n");

	i = 0;
	while (envp[i] != NULL)
	{
		new_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	new_envp[i] = ft_strdup(desired_new_env);
	new_envp[i + 1] = NULL;
	return new_envp;
}

char **ft_copy_env(char **envp)
{
	int i;
	int j;
	int k;
	int envp_len = ft_envp_len(envp);
	char **envp_copy = malloc(sizeof(char *) * (envp_len + 1));

	if (!envp_copy)
		ft_print_exit("Error: malloc failed\n");

	i = 0;
	while (envp[i] != NULL)
	{
		// 14 is the length of "declare -x " and 2 is the length of the quotes [OK]
		envp_copy[i] = malloc(ft_strlen(envp[i]) + 14);

		if (!envp_copy[i])
			ft_print_exit("Error: malloc failed\n");

		// Copy the declare -x and the environment variable [OK]
		ft_strcpy(envp_copy[i], "declare -x ");
		ft_strlcat(envp_copy[i], envp[i], ft_strlen(envp[i]) + 12);

		// Add the quotes to the value of the environment variable [OK]
		j = 0;
		while (envp_copy[i][j] != '\0')
		{
			if (envp_copy[i][j] == '=')
			{
				k = ft_strlen(envp_copy[i]);
				while (k > j)
				{
					envp_copy[i][k + 1] = envp_copy[i][k];
					k--;
				}
				envp_copy[i][j + 1] = '"';
				break;
			}
			j++;
		}
		if (ft_strchr(envp_copy[i], '='))
			ft_strlcat(envp_copy[i], "\"", ft_strlen(envp_copy[i]) + 2);	
		i++;
	}
	envp_copy[envp_len] = NULL;
	return envp_copy;
}

void ft_export(t_data *data)
{
	int i;
	char *desired_new_env;
	char **envp_copy;

	i = 0;
	desired_new_env = NULL;
	envp_copy = ft_copy_env(data->envp);

	if (data->token->next->content == NULL)
	{
		while (envp_copy[++i] != NULL)
			printf("%s\n", envp_copy[i]);
		return;
	}
	else
	{
		desired_new_env = data->token->next->content;
		if (ft_isalpha(desired_new_env[0]) == 0)
		{
			printf("minishell: export: `%s': not a valid identifier\n", desired_new_env);
			return;
		}
		else
			data->envp = ft_add_new_env(data->envp, desired_new_env);
	}
}
