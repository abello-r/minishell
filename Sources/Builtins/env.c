#include "../../Includes/minishell.h"

void ft_env(t_data *data)
{
    int x;
    x = 0;
    if (data->envp == NULL)
        printf("Error: envp is NULL\n");
    else
        while (data->envp[++x] != NULL)
            printf("%s\n", data->envp[x]);
}
