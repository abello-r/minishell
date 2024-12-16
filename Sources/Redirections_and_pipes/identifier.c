/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:43:01 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/16 22:04:41 by pausanch         ###   ########.fr       */
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
		ft_echo(data);
}

void ft_execute_commands(t_data *data)
{
	t_cmd *current = data->cmds;
	int pipe_fd[2];	   // Descriptores del pipe
	int input_fd = -1; // Entrada para el siguiente comando

	while (current)
	{
		// Manejar el comando exit de forma especial
		if (ft_strcmp(current->argv[0], "exit") == 0)
		{
			ft_exit(data);
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

		if (pid == 0) // Proceso hijo
		{
			/* ft_heredoc(data); */
			t_token *temp = data->token;
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
			char *cmd_path = ft_utils_build_command_path(data, current);

			// Configurar redirecciones de entrada
			if (input_fd != -1)
			{
				dup2(input_fd, STDIN_FILENO);
				close(input_fd);
			}

			// Configurar pipe de salida si hay siguiente comando
			if (current->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[0]);
			}

			// Configurar archivo de entrada si existe
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

			// Configurar archivo de salida si existe
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

			// Ejecutar builtin o comando externo
			if (is_builtin(current->argv[0]))
			{
				ft_execute_builtin(data);
				exit(EXIT_SUCCESS);
			}
			else
			{
				// Buscar y ejecutar comando externo


				// Expand $ vars and said command not found:
				/* if (!cmd_path)
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
				} */
				if (execve(cmd_path, current->argv, data->envp) == -1)
				{
					perror("Error ejecutando el comando");
					free(cmd_path);
					exit(EXIT_FAILURE);
				}
				free(cmd_path);
			}
		}
		else if (pid > 0) // Proceso padre
		{
			waitpid(pid, &g_status, 0);

			// Gestión de pipes
			if (input_fd != -1)
				close(input_fd);

			if (current->next)
			{
				close(pipe_fd[1]);
				input_fd = pipe_fd[0];
			}
		}
		else // Error en fork
		{
			perror("Error creando proceso hijo");
			exit(EXIT_FAILURE);
		}

		current = current->next;
	}
}
