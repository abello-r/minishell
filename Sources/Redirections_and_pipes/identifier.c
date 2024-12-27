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

// Safe free function to prevent double frees
static void safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

// Function to execute built-in commands
void ft_execute_builtin(t_data *data)
{
	if (ft_strncmp(data->cmds->argv[0], "pwd", ft_strlen("pwd")) == 0)
		ft_pwd();
	else if (ft_strncmp(data->cmds->argv[0], "env", ft_strlen("env")) == 0)
		ft_env(data);
	else if (ft_strncmp(data->cmds->argv[0], "unset", ft_strlen("unset")) == 0)
		ft_unset(data);
	else if (ft_strncmp(data->cmds->argv[0], \
		"export", ft_strlen("export")) == 0)
		ft_export(data);
	else if (ft_strncmp(data->cmds->argv[0], "cd", ft_strlen("cd")) == 0)
		ft_cd(data);
	else if (ft_strncmp(data->cmds->argv[0], "echo", ft_strlen("echo")) == 0)
		ft_echo(data);
}

// Error handling function
static void handle_error(char *msg, int exit_code)
{
	perror(msg);
	exit(exit_code);
}

// Handle input redirection
static int handle_input_redirection(t_cmd *current, int input_fd)
{
	int fd_in;

	if (input_fd != -1)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
			return (-1);
		close(input_fd);
	}
	if (current->input_file)
	{
		fd_in = open(current->input_file, O_RDONLY);
		if (fd_in < 0)
			return (-1);
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			close(fd_in);
			return (-1);
		}
		close(fd_in);
	}
	return (0);
}

// Handle output redirection
static int	handle_output_redirection(t_cmd *current)
{
	int	fd_out;

	if (current->output_file)
	{
		if (current->append)
			fd_out = open(current->output_file,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd_out = open(current->output_file,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out < 0)
			return (-1);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			close(fd_out);
			return (-1);
		}
		close(fd_out);
		return (1);
	}
	return (0);
}

// Handle all redirections
static int handle_redirections(t_cmd *current, int input_fd)
{
	if (handle_input_redirection(current, input_fd) == -1)
		return (-1);
	if (handle_output_redirection(current) == -1)
		return (-1);
	return (0);
}

// Find command path
static char *find_command_path(t_data *data, char *cmd)
{
	int i;
	char *cmd_path;
	char *tmp;
	char *temp_path;

	i = 0;
	cmd_path = NULL;
	while (data->path && data->path[i])
	{
		tmp = ft_strjoin(data->path[i], "/");
		if (!tmp)
			return (NULL);
		temp_path = ft_strjoin(tmp, cmd);
		safe_free((void **)&tmp);
		if (!temp_path)
			return (NULL);
		if (access(temp_path, X_OK) == 0)
		{
			cmd_path = temp_path;
			break;
		}
		safe_free((void **)&temp_path);
		i++;
	}
	return (cmd_path);
}

// Handle command not found for env variables
static void handle_env_command_not_found(t_data *data, char *cmd)
{
	char *tmp;
	char *env;
	char *error_msg;

	if (cmd[0] == '$' && cmd[1] != '\0')
	{
		tmp = ft_substr(cmd, 1, ft_strlen(cmd));
		if (!tmp)
			return;
		env = ft_get_env(data, tmp);
		safe_free((void **)&tmp);
		if (env)
		{
			error_msg = ft_strjoin(env, ": command not found\n");
			if (error_msg)
			{
				write(2, error_msg, ft_strlen(error_msg));
				safe_free((void **)&error_msg);
			}
			safe_free((void **)&env);
		}
	}
	else
	{
		error_msg = ft_strjoin(cmd, ": command not found\n");
		if (error_msg)
		{
			write(2, error_msg, ft_strlen(error_msg));
			safe_free((void **)&error_msg);
		}
	}
}

// Execute external command
static void execute_external_command(t_data *data, t_cmd *current,
									 int input_fd, int *pipe_fd)
{
	char *cmd_path;

	if (handle_redirections(current, input_fd) == -1)
		handle_error("Error en redirecciones", EXIT_FAILURE);
	if (current->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			handle_error("Error en pipe", EXIT_FAILURE);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	cmd_path = find_command_path(data, current->argv[0]);
	if (!cmd_path)
	{
		handle_env_command_not_found(data, current->argv[0]);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, current->argv, data->envp) == -1)
	{
		safe_free((void **)&cmd_path);
		handle_error("Error ejecutando el comando", EXIT_FAILURE);
	}
	safe_free((void **)&cmd_path);
}

// Restore standard file descriptors
static void restore_std_fds(int stdout_backup, int stdin_backup)
{
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
// Handle direct builtin execution
static void execute_builtin_direct(t_data *data, t_cmd *current)
{
	int stdout_backup;
	int stdin_backup;

	stdout_backup = -1;
	stdin_backup = -1;
	if (current->output_file)
	{
		stdout_backup = dup(STDOUT_FILENO);
		if (stdout_backup == -1)
			return;
	}
	if (current->input_file)
	{
		stdin_backup = dup(STDIN_FILENO);
		if (stdin_backup == -1)
		{
			if (stdout_backup != -1)
				close(stdout_backup);
			return;
		}
	}
	if (handle_redirections(current, -1) == -1)
	{
		if (stdout_backup != -1)
			close(stdout_backup);
		if (stdin_backup != -1)
			close(stdin_backup);
		return;
	}
	ft_execute_builtin(data);
	restore_std_fds(stdout_backup, stdin_backup);
}

// Handle child process
static void handle_child_process(t_data *data, t_cmd *current,
								 int input_fd, int *pipe_fd)
{
	if (handle_redirections(current, input_fd) == -1)
		handle_error("Error en redirecciones", EXIT_FAILURE);
	if (current->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			handle_error("Error en pipe", EXIT_FAILURE);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (is_builtin(current->argv[0]))
	{
		ft_execute_builtin(data);
		exit(EXIT_SUCCESS);
	}
	else
		execute_external_command(data, current, -1, pipe_fd);
}

// Handle command execution with fork
static int execute_command_with_fork(t_data *data, t_cmd *current,
									 int *input_fd, int pipe_fd[2])
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		handle_child_process(data, current, *input_fd, pipe_fd);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_status = 128 + WTERMSIG(status);
	}
	return (0);
}

// Handle parent cleanup
static void handle_parent_cleanup(t_cmd *current, int *input_fd, int pipe_fd[2])
{
	if (*input_fd != -1)
		close(*input_fd);
	if (current->next)
	{
		close(pipe_fd[1]);
		*input_fd = pipe_fd[0];
	}
}

// Execute a single command
static int execute_command(t_data *data, t_cmd *current,
						   int *input_fd, int pipe_fd[2])
{
	if (current->next)
	{
		if (pipe(pipe_fd) == -1)
			return (-1);
	}
	if (!is_builtin(current->argv[0]) || current->next || *input_fd != -1)
	{
		if (execute_command_with_fork(data, current, input_fd, pipe_fd) == -1)
		{
			if (current->next)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			return (-1);
		}
	}
	else
		execute_builtin_direct(data, current);
	handle_parent_cleanup(current, input_fd, pipe_fd);
	return (0);
}

// Main command execution function
void ft_execute_commands(t_data *data)
{
	t_cmd *current;
	int pipe_fd[2];
	int input_fd;

	current = data->cmds;
	input_fd = -1;
	while (current)
	{
		if (ft_strcmp(current->argv[0], "exit") == 0)
		{
			ft_exit(data);
			return;
		}
		if (execute_command(data, current, &input_fd, pipe_fd) == -1)
		{
			handle_error("Error executing command", EXIT_FAILURE);
			return;
		}
		current = current->next;
	}
}
