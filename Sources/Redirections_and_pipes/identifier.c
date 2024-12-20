/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:43:01 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/20 17:49:40 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int g_status;

void	ft_execute_builtin(t_data *data)
{
	if (ft_strncmp(data->cmds->argv[0], "pwd", ft_strlen("pwd")) == 0)
		ft_pwd();
	else if (ft_strncmp(data->cmds->argv[0], "env", ft_strlen("env")) == 0)
		ft_env(data);
	else if (ft_strncmp(data->cmds->argv[0], "unset", ft_strlen("unset")) == 0)
		ft_unset(data);
	else if (ft_strncmp(data->cmds->argv[0], "export", \
		ft_strlen("export")) == 0)
		ft_export(data);
	else if (ft_strncmp(data->cmds->argv[0], "cd", ft_strlen("cd")) == 0)
		ft_cd(data);
	else if (ft_strncmp(data->cmds->argv[0], "echo", ft_strlen("echo")) == 0)
		ft_echo(data);
}

static void	execute_external_command(t_data *data, t_cmd *current, int input_fd, int *pipe_fd)
{
	int		fd_in;
	int		fd_out;
	int		i;
	char	*cmd_path;
	char	*tmp;
	char	*temp_path;
	char	*env;

	if (input_fd != -1)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (current->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
	}
	if (current->input_file)
	{
		fd_in = open(current->input_file, O_RDONLY);
		if (fd_in < 0)
		{
			perror("Error al abrir archivo de entrada");
			exit(EXIT_FAILURE);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (current->output_file)
	{
		if (current->append)
		{
			fd_out = open(current->output_file, \
			O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else
		{
			fd_out = open(current->output_file, \
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if (fd_out < 0)
		{
			perror("Error al abrir archivo de salida");
			exit(EXIT_FAILURE);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	i = 0;
	cmd_path = NULL;
	while (data->path && data->path[i])
	{
		tmp = ft_strjoin(data->path[i], "/");
		temp_path = ft_strjoin(tmp, current->argv[0]);
		free(tmp);
		if (access(temp_path, X_OK) == 0)
		{
			cmd_path = temp_path;
			break ;
		}
		free(temp_path);
		i++;
	}
	if (!cmd_path)
	{
		if (current->argv[0][0] == '$' && current->argv[0][1] != '\0')
		{
			tmp = ft_substr(current->argv[0], 1, ft_strlen(current->argv[0]));
			env = ft_get_env(data, tmp);
			free(tmp);
			if (env)
				printf("%s: command not found3\n", env);
		}
		else
		{
			printf("%s: command not found4\n", current->argv[0]);
		}
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, current->argv, data->envp) == -1)
	{
		perror("Error ejecutando el comando");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
}

void	ft_execute_commands(t_data *data)
{
	t_cmd	*current;
	int		pipe_fd[2];
	int		input_fd;
	int		status;
	int		stdout_backup;
	int		stdin_backup;
	int		fd_out;
	int		fd_in;

	current = data->cmds;
	input_fd = -1;
	while (current)
	{
		if (ft_strcmp(current->argv[0], "exit") == 0)
		{
			ft_exit(data);
			return ;
		}
		if (is_builtin(current->argv[0]) && (!current->next && input_fd == -1))
		{
			stdout_backup = -1;
			stdin_backup = -1;
			if (current->output_file)
			{
				stdout_backup = dup(STDOUT_FILENO);
				if (current->append)
					fd_out = open(current->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					fd_out = open(current->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
			}
			if (current->input_file)
			{
				stdin_backup = dup(STDIN_FILENO);
				fd_in = open(current->input_file, O_RDONLY);
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
			ft_execute_builtin(data);
			if (stdout_backup != -1)
			{
				dup2(stdout_backup, STDOUT_FILENO);
				close(stdout_backup);
			}
			if (stdin_backup != -1)
			{
				dup2(stdin_backup, STDIN_FILENO);
				close(stdin_backup);
			}
		}
		else
		{
			if (current->next)
			{
				if (pipe(pipe_fd) == -1)
				{
					perror("Error creando pipe");
					exit(EXIT_FAILURE);
				}
			}
			pid_t pid = fork();
			if (pid == 0)
			{
				t_token *temp = data->token;
				while (temp)
				{
					if (ft_strcmp(temp->type, "HEREDOC") == 0)
					{
						ft_heredoc(data);
						int fd = open("heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
						close(fd);
						exit(EXIT_SUCCESS);
					}
					temp = temp->next;
				}

				if (is_builtin(current->argv[0]))
				{
					ft_execute_builtin(data);
					exit(EXIT_SUCCESS);
				}
				else
				{
					execute_external_command(data, current, input_fd, pipe_fd);
				}
			}
			else if (pid > 0)
			{
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					g_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					g_status = 128 + WTERMSIG(status);

				if (input_fd != -1)
					close(input_fd);

				if (current->next)
				{
					close(pipe_fd[1]);
					input_fd = pipe_fd[0];
				}
			}
			else
			{
				perror("Error creando proceso hijo");
				g_status = 1;
				exit(EXIT_FAILURE);
			}
		}
		current = current->next;
	}
}
