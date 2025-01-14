/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:49:40 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 19:04:20 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

void	execute_child_process(t_data *data, t_cmd *current,
							int *prev_pipe, int *curr_pipe)
{
	int		fd;
	t_token	*temp;

	temp = data->token;
	while (temp)
	{
		if (ft_strcmp(temp->type, "HEREDOC") == 0)
		{
			ft_heredoc(data);
			fd = open("heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			close(fd);
			exit(EXIT_SUCCESS);
		}
		temp = temp->next;
	}
	execute_child(data, current, prev_pipe, curr_pipe);
}

void	handle_pipes(int *prev_pipe, int *curr_pipe, int has_next)
{
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (has_next)
	{
		prev_pipe[0] = curr_pipe[0];
		prev_pipe[1] = curr_pipe[1];
	}
}

void	wait_last_process(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_status = 128 + WTERMSIG(status);
}

void	ft_handle_process(t_data *data, t_cmd *current, int prev_pipe[2],
						int curr_pipe[2])
{
	pid_t	pid;
	int		*pipe_in;
	int		*pipe_out;

	pid = fork();
	if (pid == -1)
	{
		ft_error_fork(curr_pipe);
		return ;
	}
	if (pid == 0)
	{
		pipe_in = NULL;
		pipe_out = NULL;
		if (prev_pipe[0] != -1)
			pipe_in = prev_pipe;
		if (current->next)
			pipe_out = curr_pipe;
		execute_child_process(data, current, pipe_in, pipe_out);
	}
	handle_pipes(prev_pipe, curr_pipe, current->next != NULL);
	if (!current->next)
		wait_last_process(pid);
}

void	ft_execute_commands(t_data *data)
{
	t_cmd	*current;
	int		prev_pipe[2];
	int		curr_pipe[2];

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	current = data->cmds;
	while (current)
	{
		if (ft_strcmp(current->argv[0], "exit") == 0)
			return (ft_exit(data));
		if (current->next && !setup_pipes(curr_pipe))
			return ;
		if (is_builtin(current->argv[0]) && !current->next
			&& prev_pipe[0] == -1)
			execute_single_builtin(data, current);
		else
			ft_handle_process(data, current, prev_pipe, curr_pipe);
		current = current->next;
	}
	while (wait(NULL) > 0)
		;
}
