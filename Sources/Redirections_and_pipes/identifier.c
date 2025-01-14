/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:49:40 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 17:57:54 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int g_status;

void ft_update_g_status(pid_t pid)
{
	int status;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_status = 128 + WTERMSIG(status);
}

void ft_execute_commands(t_data *data)
{
	t_cmd *current;
	int prev_pipe[2] = {-1, -1};
	int curr_pipe[2];
	pid_t pid;
	t_token *temp;

	temp = data->token;
	current = data->cmds;
	while (current)
	{
		if (ft_strcmp(current->argv[0], "exit") == 0)
			return (ft_exit(data));
		if (current->next && pipe(curr_pipe) == -1)
		{
			perror("pipe");
			g_status = 1;
			return ;
		}
		if (is_builtin(current->argv[0]) && !current->next && prev_pipe[0] == -1)
			execute_single_builtin(data, current);
		else
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				if (curr_pipe[0] != -1)
				{
					close(curr_pipe[0]);
					close(curr_pipe[1]);
				}
				g_status = 1;
				return ;
			}
			if (pid == 0)
			{
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
				execute_child(data, current, prev_pipe[0] != -1 ? prev_pipe : NULL, current->next ? curr_pipe : NULL);
			}
			if (prev_pipe[0] != -1)
			{
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if (current->next)
			{
				prev_pipe[0] = curr_pipe[0];
				prev_pipe[1] = curr_pipe[1];
			}
			if (!current->next)
			{
				ft_update_g_status(pid);
			}
		}
		current = current->next;
	}
	while (wait(NULL) > 0)
		;
}