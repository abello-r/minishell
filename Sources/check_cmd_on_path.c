/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_on_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 14:46:25 by abello-r          #+#    #+#             */
/*   Updated: 2024/11/22 13:02:55 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

static int	ft_count_cmd_tokens(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

static char	**ft_build_argv(t_token *token)
{
	int		i;
	int		count;
	char	**cmd_argv;

	i = 0;
	count = ft_count_cmd_tokens(token);
	cmd_argv = malloc((count + 1) * sizeof(char *));
	while (token)
	{
		cmd_argv[i++] = token->content;
		token = token->next;
	}
	cmd_argv[i] = NULL;
	return (cmd_argv);
}

int	ft_execute_cmd(t_data *data, char *command)
{
	pid_t	pid;
	int		status;
	int		exit_status;
	char	**argv;

	argv = ft_build_argv(data->token);
	pid = fork();
	if (pid == 0)
	{
		execve(command, argv, data->envp);
		ft_print_exit("EXECVE");
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			free(argv);
			return (exit_status);
		}
	}
	else
		ft_print_exit("FORK");
	return (free(argv), 0);
}

void	ft_check_cmd_on_path(t_data *data)
{
	int		i;
	char	*tmp;
	char	*tmp2;
	int		exit_status;

	i = 0;
	while (data->path[i])
	{
		tmp = ft_strjoin(data->path[i], "/");
		tmp2 = ft_strjoin(tmp, data->token->content);
		free(tmp);
		if (access(tmp2, F_OK) == 0)
		{
			exit_status = ft_execute_cmd(data, tmp2);
			if (exit_status == 0)
			{
				free(data->token->content);
				data->token->content = ft_strdup(tmp2);
				return (free(tmp2));
			}
		}	
		free(tmp2);
		i++;
	}
	printf("%s: command not found\n", data->token->content);
}
	
