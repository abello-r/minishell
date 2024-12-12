/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:43:01 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/12 19:23:40 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int g_status = 0;

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
	{
		ft_echo(data);
	}
	else if (ft_strncmp(data->cmds->argv[0], "exit", ft_strlen("exit")) == 0)
		ft_exit(data);
}

void ft_execute_commands(t_data *data)
{
	t_cmd *current = data->cmds;
	int pipe_fd[2];	   // Descriptores del pipe
	int input_fd = -1; // Entrada para el siguiente comando
	/* int status; */

	while (current)
	{
		if (/* is_builtin(current->argv[0]) && */ strcmp(current->argv[0], "exit") == 0)
		{
			ft_exit(data);
			return ;
		}

		if (strcmp(current->argv[0], "export") == 0 && !current->next && 
            !current->input_file && !current->output_file)
        {
            ft_export(data);
            return;
        }

		// Si es un builtin sin pipes ni redirecciones, ejecutar directamente
        if (is_builtin(current->argv[0]) && !current->next && 
            !current->input_file && !current->output_file)
        {
            ft_execute_builtin(data);
            return;
        }

		// Crear un pipe si hay un próximo comando
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
				int fd_in = open(current->input_file, O_RDONLY);
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
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
			}

			if (is_builtin(current->argv[0]))
			{
				ft_execute_builtin(data);
				exit(EXIT_SUCCESS);
			}
			else
			{
				// Construir el path absoluto para el comando
				char *cmd_path = NULL;
				int i = 0;

				while (data->path[i])
				{
					char *tmp;
					char *temp_path;
					tmp = ft_strjoin(data->path[i], "/");
					temp_path = ft_strjoin(tmp, current->argv[0]);
					free(tmp);
					if (access(temp_path, X_OK) == 0)
					{
						cmd_path = temp_path;
						break;
					}
					free(temp_path);
					i++;
				}

				if (!cmd_path)
				{
					if (current->argv[0][0] == '$' && current->argv[0][1] != '\0')
					{
						char *tmp = ft_substr(current->argv[0], 1, ft_strlen(current->argv[0]));
						char *env = ft_get_env(data, tmp);
						free(tmp);
						if (env)
							printf("%s: command not found\n", env);
					}
					else
						printf("%s: command not found\n", current->argv[0]);
					exit(EXIT_FAILURE);
				}
				else if (execve(cmd_path, current->argv, data->envp) == -1)
				{
					if (ft_strlen(current->argv[0]) <= 0) {
						return ;
					}
					perror("Error ejecutando el comando");
					free(cmd_path);
					exit(EXIT_FAILURE);
				}
				free(cmd_path);
			}
		}
		else if (pid > 0)
		{
			waitpid(pid, &g_status, 0); // Esperar al proceso hijo

			// Manejo de pipes
			if (input_fd != -1)
				close(input_fd); // Cerrar la entrada usada por el proceso anterior

			if (current->next)
			{
				close(pipe_fd[1]);	   // Cerrar extremo de escritura del pipe
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
