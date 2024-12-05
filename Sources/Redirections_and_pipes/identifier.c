/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/12/05 16:11:23 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../Includes/minishell.h"

int g_status = 0;

void ft_execute_builtin(t_data *data)
{
    if (ft_strncmp(data->token->content, "pwd", ft_strlen("pwd")) == 1)
        ft_pwd();
    else if (ft_strncmp(data->token->content, "env", ft_strlen("env")) == 1)
        ft_env(data);
    else if (ft_strncmp(data->token->content, "unset", ft_strlen("unset")) == 1)
        ft_unset(data);
    else if (ft_strncmp(data->token->content, "export", ft_strlen("export")) == 1)
        ft_export(data);
    else if (ft_strncmp(data->token->content, "cd", ft_strlen("cd")) == 1)
        ft_cd(data);
    else if (ft_strncmp(data->token->content, "echo", ft_strlen("echo")) == 1)
        ft_echo(data);
    else if (ft_strncmp(data->token->content, "exit", ft_strlen("exit")) == 1)
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
	ft_check_cmd_on_path(data);
    exit(1);
}

int exec_fork(t_data *data, t_token *current_token, int in_fd, int out_fd)
{
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        g_status = 1;
        return -1;
    } else if (pid == 0) {
        child_process(data, current_token, in_fd, out_fd);
    } else {
    	waitpid(pid, &g_status, 0);
	}
    return g_status;
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
            g_status = 1;
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
        g_status = 1;
        return -1;
    }

    return 0;
}

int ft_handle_redirections_and_pipes(t_data *data)
{
    int in_fd = STDIN_FILENO;
    int out_fd = STDOUT_FILENO;
    int pipe_fds[2];
    int has_pipe = 0;

    if (is_builtin(data->token->content) && !strcmp(data->token->content, "exit")) {
        ft_exit(data);
        return g_status;
    }
    if (handle_redirections(data, &in_fd, &out_fd) < 0)
        return g_status;
    if (handle_pipes(data, pipe_fds, &has_pipe) < 0)
        return g_status;

	while(data->token)
	{
		if (ft_strcmp(data->token->type, "BUILTIN") == 0) {
			printf("Builtin command found: %s\n", data->token->content);
			ft_execute_builtin(data);
		}
		else if (ft_strcmp(data->token->type, "CMD") == 0) {
			g_status = exec_fork(data, data->token, in_fd, out_fd);
		}
		if (data->token->next)
			data->token = data->token->next;
	}

    if (in_fd != STDIN_FILENO) close(in_fd);
    if (out_fd != STDOUT_FILENO) close(out_fd);
    if (has_pipe) close(pipe_fds[0]);

    return g_status;
}
void execute_commands(t_cmd *cmds)
{
    t_cmd *current = cmds;
    int pipe_fd[2];    // Descriptores del pipe
    int input_fd = -1; // Entrada para el siguiente comando

    while (current)
    {
        // Crear un pipe si hay un próximo comando
        if (current->next)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("Error creando pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork(); // Crear un proceso hijo

        if (pid == 0) // Código del hijo
        {
            // Redirección de entrada
            if (input_fd != -1)
            {
                dup2(input_fd, STDIN_FILENO); // Usar input_fd como entrada estándar
                close(input_fd);
            }

            // Redirección de salida
            if (current->next)
            {
                dup2(pipe_fd[1], STDOUT_FILENO); // Redirigir salida al pipe
                close(pipe_fd[0]);              // Cerrar extremo de lectura del pipe
            }

            // Redirección de archivos
            if (current->input_file)
            {
                int fd_in = open(current->input_file, O_RDONLY);
                if (fd_in < 0)
                {
                    perror("Error al abrir archivo de entrada");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_in, STDIN_FILENO); // Redirigir entrada estándar
                close(fd_in);
            }

            if (current->output_file)
            {
                int fd_out;
                if (current->append)
                    fd_out = open(current->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd_out = open(current->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if (fd_out < 0)
                {
                    perror("Error al abrir archivo de salida");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_out, STDOUT_FILENO); // Redirigir salida estándar
                close(fd_out);
            }

            // Ejecutar el comando
            if (execvp(current->argv[0], current->argv) == -1)
            {
                perror("Error ejecutando comando");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0) // Código del padre
        {
            int status;
            waitpid(pid, &status, 0); // Esperar al proceso hijo

            // Manejo de pipes
            if (input_fd != -1)
                close(input_fd); // Cerrar la entrada usada por el proceso anterior

            if (current->next)
            {
                close(pipe_fd[1]);     // Cerrar extremo de escritura del pipe
                input_fd = pipe_fd[0]; // Usar extremo de lectura como entrada para el próximo comando
            }
        }
        else // Error en fork
        {
            perror("Error creando proceso hijo");
            exit(EXIT_FAILURE);
        }

        // Avanzar al siguiente comando
        current = current->next;
    }
}