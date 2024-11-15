/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:48:35 by abello-r          #+#    #+#             */
/*   Updated: 2024/11/15 17:16:55 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

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

void child_process(t_data *data, t_token *current_token, int in_fd, int out_fd)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
	(void)current_token;

    if (in_fd != STDIN_FILENO) {
        if (dup2(in_fd, STDIN_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO) {
        if (dup2(out_fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        close(out_fd);
    }
	ft_execute_builtin(data);
    exit(1);
}

int exec_fork(t_data *data, t_token *current_token, int in_fd, int out_fd)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        child_process(data, current_token, in_fd, out_fd);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
    }
    return 1;
}

int handle_redirections(t_data *data, int *in_fd, int *out_fd)
{
    t_token *current_token = data->token;

    while (current_token) {
        if (!strcmp(current_token->type, "OUT") || !strcmp(current_token->type, "APPEND")) {
            *out_fd = !strcmp(current_token->type, "OUT") ?
                      open(current_token->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0777) :
                      open(current_token->next->content, O_WRONLY | O_CREAT | O_APPEND, 0777);
        } else if (!strcmp(current_token->type, "INPUT")) {
            *in_fd = open(current_token->next->content, O_RDONLY);
        }

        if (*out_fd < 0 || *in_fd < 0) {
            perror("open");
            return -1;
        }

        current_token = current_token->next;
    }
    return 0;
}

int handle_pipes(t_data *data, int *pipe_fds, int *has_pipe)
{
    t_token *current_token = data->token;

    while (current_token) {
        if (!strcmp(current_token->type, "PIPE")) {
            *has_pipe = 1;
        }
        current_token = current_token->next;
    }

    if (*has_pipe && pipe(pipe_fds) < 0) {
        perror("pipe");
        return -1;
    }

    return 0;
}

int ft_handle_redirections_and_pipes(t_data *data)
{
    int in_fd;
    int out_fd;
    int pipe_fds[2];
    int has_pipe;
    int exit_status;

	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	has_pipe = 0;
    if (is_builtin(data->token->content) && !strcmp(data->token->content, "exit")) {
        ft_exit(data);
        return 0;
    }
    if (handle_redirections(data, &in_fd, &out_fd) < 0)
        return -1;
    if (handle_pipes(data, pipe_fds, &has_pipe) < 0)
        return -1;
    exit_status = exec_fork(data, data->token, in_fd, out_fd);
    if (in_fd != STDIN_FILENO) close(in_fd);
    if (out_fd != STDOUT_FILENO) close(out_fd);
    if (has_pipe) close(pipe_fds[0]);
    return exit_status;
}
