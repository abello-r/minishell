/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:43:01 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/16 17:43:21 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int g_status = 0;

char	*ft_save_in_tmp_file(char *heredoc)
{
	ssize_t	written;
	int		fd;
	char	*tmp_file;

	tmp_file = ft_strdup("heredoc.tmp");
	if (!tmp_file)
		return (NULL);
    fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(tmp_file);	
		return (NULL);
	}
	written = write(fd, heredoc, ft_strlen(heredoc));
	close(fd);
	if (written < 0)
	{
		free(tmp_file);
		return (NULL);
	}
	return (tmp_file);
}

/* int		ft_exec_with_tmp_file(t_data *data, char *tmp_file)
{
	
	return (0);
} */

void ft_heredoc(t_data *data)
{
	int i = 0;
	char *outfile = NULL;
	int pipe_fd[2];

	while (data->cmds->argv[i])
	{
		if (ft_strncmp(data->cmds->argv[i], "<<", 2) == 0)
		{
			if (data->cmds->argv[i + 1])
				outfile = data->cmds->argv[i + 1];
			break;
		}
		i++;
	}

	if (!data->cmds->argv[i])
		return;

	if (pipe(pipe_fd) < 0)
	{
		perror("Pipe error");
		exit(EXIT_FAILURE);
	}

	char *delimiter = ft_substr(data->cmds->argv[i], 2, ft_strlen(data->cmds->argv[i]));
	char *line = NULL;
	char *heredoc = ft_strdup("");

	// Leer las líneas del heredoc
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = readline("");

		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		char *temp = ft_strjoin(heredoc, line);
		free(heredoc);
		heredoc = ft_strjoin(temp, "\n");
		free(temp);
		free(line);
	}

	// Manejo del archivo de salida
	if (outfile)
	{
		int fd_out;
		if (data->cmds->append)
			fd_out = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if (fd_out < 0)
		{
			perror("Error al abrir archivo de salida");
			exit(EXIT_FAILURE);
		}

		// Escribir el heredoc en el archivo de salida
		ssize_t written = write(fd_out, heredoc, ft_strlen(heredoc));
		if (written < 0)
		{
			perror("Error escribiendo en archivo");
			exit(EXIT_FAILURE);
		}

		// Asegúrate de cerrar el archivo después de escribir
		close(fd_out);

		// Si el archivo no se ha actualizado, asegúrate de hacer un "flush"
		if (fsync(fd_out) < 0)
		{
			// perror("Error al hacer flush en el archivo");
			exit(EXIT_FAILURE);
		}
	}

	char *tmp_file = ft_save_in_tmp_file(heredoc);
	/* ft_exec_with_tmp_file(data, tmp_file); */

	printf("TMP FILE: %s\n", tmp_file);

	if (ft_strcmp(data->cmds->argv[0], "grep") == 0)
	{
		ssize_t written = write(pipe_fd[1], heredoc, ft_strlen(heredoc));
		if (written < 0)
		{
			perror("Error writing to pipe");
			exit(EXIT_FAILURE);
		}
		char buffer[4096];
		close(pipe_fd[1]);

		ssize_t bytes_read;
		while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
		{
			write(STDOUT_FILENO, buffer, bytes_read);
		}
		close(pipe_fd[0]);
	}

	close(pipe_fd[1]);
	free(delimiter);
	free(heredoc);
}

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
					exit(EXIT_SUCCESS);
				}
				temp = temp->next;
			}
			
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
				char *cmd_path = NULL;
				int i = 0;

				while (data->path[i])
				{
					char *tmp = ft_strjoin(data->path[i], "/");
					char *temp_path = ft_strjoin(tmp, current->argv[0]);
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
