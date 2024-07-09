#include "../../Includes/minishell.h"

void ft_env(t_data *data)
{
    int x;
    x = 0;
    if (data->envp == NULL)
        printf("Error: envp is NULL\n");
    else
        while (data->envp[++x] != NULL) {
			if (!ft_strchr(data->envp[x], '='))
				continue;
            printf("%s\n", data->envp[x]);
		}
}
