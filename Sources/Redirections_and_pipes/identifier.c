/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:49:40 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/20 18:52:19 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

static void	perror_auxiliar(char *str, int code)
{
	perror(str);
	exit(code);
}

/*
	Here we established if the redir is for input or output,
	Depends on that, we make the choice about how to handle it.
*/
static void	setup_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
			perror_auxiliar(cmd->input_file, 1);
		else if (dup2(fd, STDIN_FILENO) == -1)
			perror_auxiliar("dup2", 1);
		close(fd);
	}
	if (cmd->output_file)
	{
		if (cmd->append)
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			perror_auxiliar(cmd->output_file, 1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror_auxiliar("dup2", 1);
		close(fd);
	}
}

/*
   1. Sets up input redirection from previous pipe if it exists
   2. Sets up output redirection to next pipe if it exists
   3. Closes all pipe file descriptors to prevent leaks
*/
void	pipes_handler(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe && dup2(prev_pipe[0], STDIN_FILENO) == -1)
		perror_auxiliar("dup2", 1);
	if (pipe_fd && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		perror_auxiliar("dup2", 1);
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

	pipes_handler(prev_pipe, pipe_fd);
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
			free(cmd_path);
			perror_auxiliar(cmd_path, 126);
		}
	}
}

// TODO: ADD A COMMENT HERE
static void	execute_single_builtin(t_data *data, t_cmd *cmd)
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

void	pid_handler_aux(int *p_pipe, int *c_pipe, t_cmd *current, pid_t pid)
{
	int	status;

	status = 0;
	if (p_pipe[0] != -1)
	{
		close(p_pipe[0]);
		close(p_pipe[1]);
	}
	if (current->next)
	{
		p_pipe[0] = c_pipe[0];
		p_pipe[1] = c_pipe[1];
	}
	if (!current->next)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_status = 128 + WTERMSIG(status);
	}
}

pid_t	pid_creation(int *curr_pipe)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (curr_pipe)
		{
			close(curr_pipe[0]);
			close(curr_pipe[1]);
		}
		g_status = 1;
		return (-1);
	}
	return (pid);
}

void	heredoc_handler(t_token *temp, t_data *data)
{
	int		status;
	int		fd;

	status = 0;
	fd = 0;
	if (ft_strcmp(temp->type, "HEREDOC") == 0)
	{
		ft_heredoc(data);
		fd = open("heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		close(fd);
		exit(EXIT_SUCCESS);
	}
}

void	pid_handler(t_data *data, t_cmd *current, int *p_pipe, int *c_pipe)
{
	t_token	*temp;
	int		*prev_pipe_ptr;
	int		*curr_pipe_ptr;
	pid_t	pid;

	prev_pipe_ptr = NULL;
	curr_pipe_ptr = NULL;
	temp = data->token;
	pid = pid_creation(c_pipe);
	if (pid == -1)
		return ;
	else if (pid == 0)
	{
		while (temp)
		{
			heredoc_handler(temp, data);
			temp = temp->next;
		}
		if (p_pipe[0] != -1)
			prev_pipe_ptr = p_pipe;
		if (current->next)
			curr_pipe_ptr = c_pipe;
		execute_child(data, current, prev_pipe_ptr, curr_pipe_ptr);
	}
	pid_handler_aux(p_pipe, c_pipe, current, pid);
}

void	ft_execute_commands(t_data *data)
{
	t_cmd	*current;
	int		prev_pipe[2];
	int		curr_pipe[2];

	current = data->cmds;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	while (current)
	{
		if (ft_strcmp(current->argv[0], "exit") == 0)
			return (ft_exit(data));
		if (current->next && pipe(curr_pipe) == -1)
		{
			g_status = 1;
			return (perror("pipe"));
		}
		if (is_builtin(current->argv[0])
			&& !current->next && prev_pipe[0] == -1)
			execute_single_builtin(data, current);
		else
			pid_handler(data, current, prev_pipe, curr_pipe);
		current = current->next;
	}
	while (wait(NULL) > 0)
		continue ;
}
