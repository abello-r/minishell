/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:12:38 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/19 18:47:26 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"
#include <linux/limits.h>
#include <errno.h>

extern int	g_status;

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

static int	find_oldpwd(char **envp)
{
	int	i;

	i = 0;
	while (ft_strncmp(envp[i], "OLDPWD", 6) != 0)
		i++;
	return (i);
}

static char	*ft_go_to(char *dir_path, t_data *data)
{
	char	*current_dir;
	char	buff[PATH_MAX];

	if (ft_strncmp(dir_path, "OLDPWD", 6) == 0)
	{
		dir_path = ft_get_env(data, "OLDPWD");
		if (!dir_path)
			return (NULL);
	}
	if (access(dir_path, F_OK) != 0)
	{
		perror("");
		g_status = 1;
		return (NULL);
	}
	current_dir = getcwd(buff, PATH_MAX);
	if (chdir(dir_path) == -1) {
		perror("");
		g_status = 1;
		return (NULL);
	}
	else
	{
		data->envp[find_oldpwd(data->envp)] = \
		ft_strjoin("OLDPWD=", current_dir);
	}
	current_dir = getcwd(buff, PATH_MAX);
	return (current_dir);
}

static char	*ft_get_env_dir(t_data *data, char *d_dir)
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
	home_dir = ft_get_env_dir(data, "HOME");
	desired_path = NULL;
	if (token_counter >= 2)
	{
		printf("Too many arguments\n");
		g_status = 1;
		return ;
	}
	if (data->token->next)
		desired_path = data->token->next->content;
	if (!desired_path)
		ft_go_to(home_dir, data);
	else if (desired_path[0] == '-' && !desired_path[1])
		ft_go_to("OLDPWD", data);
	else if (desired_path[0] == '~' && !desired_path[1])
		ft_go_to(home_dir, data);
	else
		ft_go_to(desired_path, data);
}
