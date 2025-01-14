/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_identifier2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 18:09:06 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 19:04:52 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

void	handle_command_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

void	ft_check_fd(int fd, t_cmd *cmd)
{
	if (fd == -1)
	{
		perror(cmd->input_file);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
}

void	setup_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		ft_check_fd(fd, cmd);
	}
	if (cmd->output_file)
	{
		if (cmd->append)
		{
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else
		{
			fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		ft_check_fd(fd, cmd);
	}
}

int	setup_pipes(int *curr_pipe)
{
	if (pipe(curr_pipe) == -1)
	{
		perror("pipe");
		g_status = 1;
		return (0);
	}
	return (1);
}

void	ft_error_fork(int *curr_pipe)
{
	perror("fork");
	if (curr_pipe[0] != -1)
	{
		close(curr_pipe[0]);
		close(curr_pipe[1]);
	}
	g_status = 1;
}
