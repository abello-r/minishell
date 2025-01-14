/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_identifier.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:37:52 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 19:05:57 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

void	ft_execute_builtin(t_data *data)
{
	if (ft_strncmp(data->cmds->argv[0], "pwd", ft_strlen("pwd")) == 0)
		ft_pwd();
	else if (ft_strncmp(data->cmds->argv[0], "env", ft_strlen("env")) == 0)
		ft_env(data);
	else if (ft_strncmp(data->cmds->argv[0], "unset", ft_strlen("unset")) == 0)
		ft_unset(data);
	else if (ft_strncmp(data->cmds->argv[0], "export",
			ft_strlen("export")) == 0)
		ft_export(data);
	else if (ft_strncmp(data->cmds->argv[0], "cd", ft_strlen("cd")) == 0)
		ft_cd(data);
	else if (ft_strncmp(data->cmds->argv[0], "echo", ft_strlen("echo")) == 0)
		ft_echo(data);
}

char	*find_command_path(t_data *data, char *cmd)
{
	int		i;
	char	*cmd_path;
	char	*tmp;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (data->path && data->path[i])
	{
		tmp = ft_strjoin(data->path[i], "/");
		if (!tmp)
			return (NULL);
		cmd_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

void	handle_redirections_and_pipes(int *prev_pipe, int *pipe_fd)
{
	if ((prev_pipe && dup2(prev_pipe[0], STDIN_FILENO) == -1)
		|| (pipe_fd && dup2(pipe_fd[1], STDOUT_FILENO) == -1))
	{
		perror("dup2");
		exit(1);
	}
	if (prev_pipe)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

void	execute_child(t_data *data, t_cmd *cmd, int *prev_pipe, int *pipe_fd)
{
	char	*cmd_path;

	handle_redirections_and_pipes(prev_pipe, pipe_fd);
	setup_redirections(cmd);
	if (is_builtin(cmd->argv[0]))
	{
		ft_execute_builtin(data);
		exit(0);
	}
	else
	{
		cmd_path = find_command_path(data, cmd->argv[0]);
		if (!cmd_path)
			handle_command_not_found(cmd->argv[0]);
		if (execve(cmd_path, cmd->argv, data->envp) == -1)
		{
			perror(cmd_path);
			free(cmd_path);
			exit(126);
		}
	}
}

void	execute_single_builtin(t_data *data, t_cmd *cmd)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = -1;
	stdout_backup = -1;
	if (cmd->input_file)
		stdin_backup = dup(STDIN_FILENO);
	if (cmd->output_file)
		stdout_backup = dup(STDOUT_FILENO);
	setup_redirections(cmd);
	ft_execute_builtin(data);
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}
