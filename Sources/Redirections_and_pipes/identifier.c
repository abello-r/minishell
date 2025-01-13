/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 17:49:40 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/13 11:36:03 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int g_status;

//hay que cambiar una cosa mas abajo, esta indicado donde

void ft_execute_builtin(t_data *data)
{
    if (ft_strncmp(data->cmds->argv[0], "pwd", ft_strlen("pwd")) == 0)
        ft_pwd();
    else if (ft_strncmp(data->cmds->argv[0], "env", ft_strlen("env")) == 0)
        ft_env(data);
    else if (ft_strncmp(data->cmds->argv[0], "unset", ft_strlen("unset")) == 0)
        ft_unset(data);
    else if (ft_strncmp(data->cmds->argv[0], "export", ft_strlen("export")) == 0)
        ft_export(data);
    else if (ft_strncmp(data->cmds->argv[0], "cd", ft_strlen("cd")) == 0)
        ft_cd(data);
    else if (ft_strncmp(data->cmds->argv[0], "echo", ft_strlen("echo")) == 0)
        ft_echo(data);
}

static char *find_command_path(t_data *data, char *cmd)
{
    char *cmd_path;
    char *tmp;
    int i;

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

static void handle_command_not_found(char *cmd)
{
    ft_putstr_fd(cmd, 2);
    ft_putendl_fd(": command not found", 2);
    exit(127);
}

static void setup_redirections(t_cmd *cmd)
{
    int fd;

    // Handle input redirection
    if (cmd->input_file)
    {
        fd = open(cmd->input_file, O_RDONLY);
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

    // Handle output redirection
    if (cmd->output_file)
    {
        fd = open(cmd->output_file, 
            O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC), 
            0644);
        if (fd == -1)
        {
            perror(cmd->output_file);
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(fd);
    }
}

static void execute_child(t_data *data, t_cmd *cmd, int *prev_pipe, int *pipe_fd)
{
    char *cmd_path;

    // Setup pipes
    if (prev_pipe && dup2(prev_pipe[0], STDIN_FILENO) == -1)
    {
        perror("dup2");
        exit(1);
    }
    if (pipe_fd && dup2(pipe_fd[1], STDOUT_FILENO) == -1)
    {
        perror("dup2");
        exit(1);
    }

    // Close all pipe fds
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

    // Setup redirections
    setup_redirections(cmd);

    // Execute builtin or external command
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

static void execute_single_builtin(t_data *data, t_cmd *cmd)
{
    int stdin_backup = -1;
    int stdout_backup = -1;

    // Backup standard fds if needed
    if (cmd->input_file)
        stdin_backup = dup(STDIN_FILENO);
    if (cmd->output_file)
        stdout_backup = dup(STDOUT_FILENO);

    // Setup redirections
    setup_redirections(cmd);

    // Execute builtin
    ft_execute_builtin(data);

    // Restore standard fds
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

static void close_pipe(int pipe_fd[2])
{
    if (pipe_fd)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
}

void ft_execute_commands(t_data *data)
{
    t_cmd *current;
    int prev_pipe[2] = {-1, -1};
    int curr_pipe[2];
    pid_t pid;

    current = data->cmds;
	//print_commands(current);
    while (current)
    {
        // Handle exit command
        if (ft_strcmp(current->argv[0], "exit") == 0)
        {
            ft_exit(data);
            return;
        }
        // Create pipe if needed
        if (current->next && pipe(curr_pipe) == -1)
        {
            perror("pipe");
			g_status = 1;
            return;
        }
        // For single builtin without pipes, execute directly
        if (is_builtin(current->argv[0]) && !current->next && prev_pipe[0] == -1)
            execute_single_builtin(data, current);
        else
        {
            // Fork and execute command
            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                close_pipe(curr_pipe);
				g_status = 1;
                return;
            }
            if (pid == 0)
            {
                execute_child(data, current, 
                    prev_pipe[0] != -1 ? prev_pipe : NULL, // ESTO HAY QUE CAMBIARLO
                    current->next ? curr_pipe : NULL);
            }
            // Parent process
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
            // Wait for child if it's the last command
            if (!current->next)
            {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    g_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    g_status = 128 + WTERMSIG(status);
            }
        }
        current = current->next;
    }
    // Wait for all remaining child processes
    while (wait(NULL) > 0)
        ;
}
