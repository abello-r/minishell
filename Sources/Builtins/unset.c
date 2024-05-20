#include "../../Includes/minishell.h"
char    *ft_get_key(char *token)
{
    int     i;
    char    *key;
    i = 0;
    while (token[i] != '=')
        i++;
    key = ft_substr(token, 0, i);
    return (key);
}

char    **ft_create_copy_without_key(t_data *data, char *key)
{
    char    **copy;
    int     i;
    int     j;
    i = 0;
    j = 0;
    while (data->envp[i] != NULL)
        i++;
    copy = (char **)malloc(sizeof(char *) * i);
    i = 0;
    while (data->envp[i] != NULL)
    {
        if (ft_strncmp(ft_get_key(data->envp[i]), key, ft_strlen(key)) != 0)
        {
            copy[j] = ft_strdup(data->envp[i]);
            j++;
        }
        i++;
    }
    copy[j] = NULL;
    return (copy);
}

int ft_str_include_equal(char *str)
{
    int i;
    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '=')
            return (1);
        i++;
    }
    return (0);
}

void    ft_unset(t_data *data, char *str)
{
    char    *key;
    char    **copy;
    int     i;
    i = 0;
    if (data->envp == NULL || str == NULL)
        return ;
    if (ft_str_include_equal(str) == 1)
    {
        printf("minishell: unset: `%s': not a valid identifier\n", str);
        return ;
    }
    while (data->envp[i] != NULL)
    {
        key = ft_get_key(data->envp[i]);
        if (ft_strcmp(key, str) == 0)
        {
            copy = ft_create_copy_without_key(data, key);
            free(data->envp);
            data->envp = copy;
            break ;
        }
        i++;
    }
}
