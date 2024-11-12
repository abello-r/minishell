/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:48:35 by abello-r          #+#    #+#             */
/*   Updated: 2024/11/12 15:49:45 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

/*  void ft_execute_builtin(t_data *data)
{
    if (ft_strncmp(data->token->content, "pwd", ft_strlen("pwd")) == 0)
        ft_pwd();
    else if (ft_strncmp(data->token->content, "env", ft_strlen("env")) == 0)
        ft_env(data);
    else if (ft_strncmp(data->token->content, "unset", ft_strlen("unset")) == 0)
        ft_unset(data);
    else if (ft_strncmp(data->token->content, "export", ft_strlen("export")) == 0)
        ft_export(data);
    else if (ft_strncmp(data->token->content, "cd", ft_strlen("cd")) == 0)
        ft_cd(data);
    else if (ft_strncmp(data->token->content, "echo", ft_strlen("echo")) == 0)
        ft_echo(data);
    else if (ft_strncmp(data->token->content, "exit", ft_strlen("exit")) == 0)
        ft_exit(data);
    else
		ft_check_cmd_on_path(data);
}  

int	ft_handle_redirections_and_pipes(t_data *data)
{
	(void)data;
	ft_execute_builtin(data);
	
	//printf("Redirections and pipes handled\n");
	return (0);

} */

void ft_execute_builtin(t_data *data)
{
    if (ft_strncmp(data->token->content, "pwd", ft_strlen("pwd")) == 0)
        ft_pwd();
    else if (ft_strncmp(data->token->content, "env", ft_strlen("env")) == 0)
        ft_env(data);
    else if (ft_strncmp(data->token->content, "unset", ft_strlen("unset")) == 0)
        ft_unset(data);
    else if (ft_strncmp(data->token->content, "export", ft_strlen("export")) == 0)
        ft_export(data);
    else if (ft_strncmp(data->token->content, "cd", ft_strlen("cd")) == 0)
        ft_cd(data);
    else if (ft_strncmp(data->token->content, "echo", ft_strlen("echo")) == 0)
        ft_echo(data);
    else if (ft_strncmp(data->token->content, "exit", ft_strlen("exit")) == 0)
        ft_exit(data);
    else
        ft_check_cmd_on_path(data);
}

int handle_redirections(t_data *data, int *in_fd, int *out_fd)
{
    t_token *current_token = data->token;

    while (current_token)
    {
        if (strcmp(current_token->type, "OUT") == 0 || strcmp(current_token->type, "APPEND") == 0)
            *out_fd = (strcmp(current_token->type, "OUT") == 0) ?
                      open(current_token->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644) :
                      open(current_token->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (strcmp(current_token->type, "INPUT") == 0)
            *in_fd = open(current_token->next->content, O_RDONLY);

        if (*out_fd < 0 || *in_fd < 0)
            return -1;

        current_token = current_token->next;
    }
    return 0;
}

int handle_pipes(t_data *data, int *pipe_fds, int *has_pipe)
{
    t_token *current_token = data->token;

    while (current_token)
    {
        if (strcmp(current_token->type, "PIPE") == 0)
            *has_pipe = 1;

        current_token = current_token->next;
    }

    if (*has_pipe && pipe(pipe_fds) < 0)
        return -1;

    return 0;
}

int execute_command(t_data *data, int in_fd, int out_fd, int *pipe_fds, int has_pipe)
{
    pid_t pid;
    int status;
    int exit_status = 0;

    if ((pid = fork()) == 0)
    {
        if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (has_pipe)
        {
            dup2(pipe_fds[1], STDOUT_FILENO);
            close(pipe_fds[0]);
			close(pipe_fds[1]); ///
        }
        else if (out_fd != STDOUT_FILENO)
        {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        if (is_builtin(data->token->content))
            ft_execute_builtin(data);
        else
            ft_check_cmd_on_path(data);
        _exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            exit_status = WEXITSTATUS(status);
        if (has_pipe)
        {
            close(pipe_fds[1]);
            in_fd = pipe_fds[0];
        }
    }
    else
        return (-1);

    return (exit_status);
}

int ft_handle_redirections_and_pipes(t_data *data)
{
    int in_fd = STDIN_FILENO;
    int out_fd = STDOUT_FILENO;
    int pipe_fds[2];
    int has_pipe = 0;
    int exit_status;

    if (is_builtin(data->token->content) && strcmp(data->token->content, "exit") == 0)
        return (ft_exit(data), 0);

    if (handle_redirections(data, &in_fd, &out_fd) < 0)
        return -1;

    if (handle_pipes(data, pipe_fds, &has_pipe) < 0)
        return -1;

    if (has_pipe)
    {
        if (pipe(pipe_fds) == -1)
            return -1;
    }

    exit_status = execute_command(data, in_fd, out_fd, pipe_fds, has_pipe);

    if (in_fd != STDIN_FILENO)
        close(in_fd);
    if (out_fd != STDOUT_FILENO)
        close(out_fd);
    if (has_pipe)
        close(pipe_fds[0]);

    return exit_status;
}
