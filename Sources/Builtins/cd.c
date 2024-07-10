/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:12:38 by abello-r          #+#    #+#             */
/*   Updated: 2024/07/10 02:45:22 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"
#include <limits.h>

int	ft_count_tokens(t_data *data)
{
	int		count;
	t_token	*current;

	current = data->token;
	count = 0;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count - 1);
}

int	find_oldpwd(char **envp)
{
	int	i;

	i = 0;
	while (ft_strncmp(envp[i], "OLDPWD", 6) != 0)
		i++;
	return (i);
}

char	*ft_go_to(char *directory_path, t_data *data)
{
	char	*current_dir;
	char	buff[PATH_MAX];

	if (ft_strncmp(directory_path, "OLDPWD", 6) == 0)
	{
		directory_path = ft_get_env(data, "OLDPWD");
		if (!directory_path)
			return (NULL);
	}
	current_dir = getcwd(buff, PATH_MAX);
	if (chdir(directory_path) == -1)
		printf("minishell: cd: %s: No such file or directory\n", \
			directory_path);
	else
	{
		data->envp[find_oldpwd(data->envp)] = \
		ft_strjoin("OLDPWD=", current_dir);
	}
	current_dir = getcwd(buff, PATH_MAX);
	return (current_dir);
}

char	*ft_get_env_dir(t_data *data, char *d_dir)
{
	int		i;
	char	*tmp_dir;

	i = 0;
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], d_dir, ft_strlen(d_dir)))
		{
			tmp_dir = ft_strdup(data->envp[i] + ft_strlen(d_dir) + 1);
			return (tmp_dir);
		}
		i++;
	}
	return (NULL);
}

void	ft_cd(t_data *data)
{
	int		token_counter;
	char	*desired_path;
	char	*home_dir;

	token_counter = ft_count_tokens(data);
	desired_path = data->token->next->content;
	home_dir = ft_get_env_dir(data, "HOME");
	if (token_counter > 2)
		ft_print_exit("minishell: cd: too many arguments\n");
	if (!desired_path)
		ft_go_to(home_dir, data);
	else if (desired_path[0] == '-' && !desired_path[1])
		ft_go_to("OLDPWD", data);
	else
		ft_go_to(desired_path, data);
}
