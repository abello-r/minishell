/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:40:54 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 13:21:14 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static int	ft_exec_with_tmp_file(t_data *data, char *tmp_file, char *outfile)
{
	int		fd_out;
	char	*path_cmd;
	char	**tmp_argv;

	path_cmd = ft_utils_build_command_path(data, data->cmds);
	if (!path_cmd)
		return (1);
	tmp_argv = ft_utils_strcpy_turbo(data->cmds->argv, tmp_file);
	fd_out = ft_utils_choose_fd_out(data, outfile);
	if (fd_out < 0 || !tmp_argv)
	{
		free(path_cmd);
		return (1);
	}
	else if (execve(path_cmd, tmp_argv, data->envp) == -1)
	{
		free(path_cmd);
		ft_utils_free_double_pointer(tmp_argv);
		exit(EXIT_FAILURE);
	}
	close(fd_out);
	free(path_cmd);
	ft_utils_free_double_pointer(tmp_argv);
	return (0);
}

static char	*ft_save_in_tmp_file(char *heredoc)
{
	ssize_t	written;
	int		fd;
	char	*tmp_file;

	tmp_file = ft_strdup("heredoc.tmp");
	if (!tmp_file)
		return (NULL);
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(tmp_file);
		return (NULL);
	}
	written = write(fd, heredoc, ft_strlen(heredoc));
	close(fd);
	if (written < 0)
	{
		free(tmp_file);
		return (NULL);
	}
	return (tmp_file);
}

static char	*ft_get_heredoc_buffer(t_data *data, int i)
{
	char	*tmp;
	char	*line;
	char	*buffer;
	char	*lim;

	tmp = NULL;
	line = NULL;
	buffer = ft_strdup("");
	lim = ft_substr(data->cmds->argv[i], 2, ft_strlen(data->cmds->argv[i]));
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = readline("");
		if (!line || ft_strcmp(line, lim) == 0)
		{
			free(line);
			break ;
		}
		tmp = ft_strjoin(buffer, line);
		free(buffer);
		buffer = ft_strjoin(tmp, "\n");
		free(tmp);
		free(line);
	}
	return (free(lim), buffer);
}

void	ft_heredoc(t_data *data)
{
	int		i;
	int		pipe_fd[2];
	char	*outfile;
	char	*buffer;
	char	*tmp_file;

	i = 0;
	outfile = NULL;
	while (data->cmds->argv[i])
	{
		if (ft_strncmp(data->cmds->argv[i], "<<", 2) == 0)
		{
			if (data->cmds->argv[i + 1])
				outfile = data->cmds->argv[i + 1];
			break ;
		}
		i++;
	}
	if (pipe(pipe_fd) < 0)
		return ;
	buffer = ft_get_heredoc_buffer(data, i);
	tmp_file = ft_save_in_tmp_file(buffer);
	ft_exec_with_tmp_file(data, tmp_file, outfile);
	close(pipe_fd[1]);
	free(buffer);
}
