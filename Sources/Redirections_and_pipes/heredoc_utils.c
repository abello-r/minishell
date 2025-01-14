/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:46:49 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 13:20:57 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	ft_utils_choose_fd_out(t_data *data, char *outfile)
{
	int	fd_out;

	if (outfile)
	{
		if (data->cmds->append)
			fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd_out, STDOUT_FILENO);
	}
	else
		fd_out = STDOUT_FILENO;
	return (fd_out);
}

char	**ft_utils_strcpy_turbo(char **src, char *tmp_file)
{
	int		i;
	int		len;
	char	**dst;

	i = 0;
	len = 1;
	if (!src)
		return (NULL);
	while (src[len] && ft_strncmp(src[len], "<<", 2) != 0)
		len++;
	dst = malloc(sizeof(char *) * (len + 2));
	if (!dst)
		return (NULL);
	dst[0] = ft_strdup(src[0]);
	while (i < len)
	{
		dst[i] = ft_strdup(src[i]);
		i++;
	}
	dst[i] = ft_strdup(tmp_file);
	dst[i + 1] = NULL;
	return (dst);
}

static void	ft_utils_cmd_not_found(char *cmd_path, t_data *data, t_cmd *current)
{
	char	*tmp;
	char	*env;

	if (!cmd_path)
	{
		if (current->argv[0][0] == '$' && current->argv[0][1] != '\0')
		{
			tmp = ft_substr(current->argv[0], 1, ft_strlen(current->argv[0]));
			env = ft_get_env(data, tmp);
			free(tmp);
			if (env)
				printf("%s: command not found\n", env);
		}
		else if (current->argv[0][0] != '<' && current->argv[0][1] != '<')
			printf("%s: command not found\n", current->argv[0]);
		exit(EXIT_FAILURE);
	}
}

char	*ft_utils_build_command_path(t_data *data, t_cmd *current)
{
	int		i;
	char	*cmd_path;
	char	*tmp_path;
	char	*tmp;

	i = 0;
	cmd_path = NULL;
	while (data->path[i])
	{
		tmp = ft_strjoin(data->path[i], "/");
		tmp_path = ft_strjoin(tmp, current->argv[0]);
		free(tmp);
		if (access(tmp_path, X_OK) == 0)
		{
			cmd_path = tmp_path;
			break ;
		}
		free(tmp_path);
		i++;
	}
	ft_utils_cmd_not_found(cmd_path, data, current);
	return (cmd_path);
}
