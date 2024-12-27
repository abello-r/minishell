/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:02:10 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/20 20:05:37 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CYAN	"\x1b[36m"
#define YELLOW	"\x1b[33m"
#define RESET	"\e[0m"
#include "../Includes/minishell.h"

int g_status = 0;

void	


cleanup_data(t_data *data)
{
    if (data->envp)
    {
        ft_utils_free_double_pointer(data->envp);
		data->envp = NULL;
    }
    if (data->path)
    {
		ft_utils_free_double_pointer(data->path);
		data->path = NULL;
    }
	if (data->token)
	{
		ft_free_tokens(data->head);
		data->token = NULL;
		data->head = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		ret;
	t_data	data;

	(void)argv;
	(void)argc;
	initializer(envp, &data);
	ret = ft_loop(&data);
	cleanup_data(&data);
	if (ret)
		return (1);
	else
		return (0);
}

void	ft_free_commands(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current)
	{
		next = current->next;
		if (current->argv)
			ft_utils_free_double_pointer(current->argv);
		if (current->input_file)
			free(current->input_file);
		if (current->output_file)
			free(current->output_file);
		free(current);
		current = next;
	}

}

void	ft_free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
	//free(current);
}

int	ft_loop(t_data *data)
{
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		data->input = readline(YELLOW "M" CYAN "inishell" "\x1b[33mツ " RESET);
		if (!data->input)
		{
			printf("\033[Fminishell$ exit\n");
			exit(EXIT_FAILURE);
		}
		if (ft_pair_quotation_check(data))
		{
			free(data->input);
			continue ;
		}
		if (ft_is_empty(data->input) != 1)
		{
			parser(data);
			add_history(data->input);
			data->cmds = parse_tokens_to_commands(data->token);
			ft_execute_commands(data);
			ft_free_commands(data->cmds);
			ft_free_tokens(data->head);
			free(data->input);
		}
	}
	return (0);
}

int	ft_character_counter(char *str, char c)
{
	int	count;
	int	total;

	count = 0;
	total = 0;
	while (str[count])
	{
		if (str[count] == c)
			total++;
		count++;
	}
	return (total);
}

int	is_builtin(char *content)
{
	int	i;

	i = ft_strlen(content);

	if ((content[0] == 'c' && ft_strlen(content) == 2)
		|| ft_strlen(content) >= 3)
	{
		if (ft_strncmp(content, "pwd", i) == 0 \
			|| ft_strncmp(content, "env", i) == 0 \
			|| ft_strncmp(content, "unset", i) == 0 \
			|| ft_strncmp(content, "export", i) == 0 \
			|| ft_strncmp(content, "cd", i) == 0 \
			|| ft_strncmp(content, "echo", i) == 0 \
			|| ft_strncmp(content, "exit", i) == 0)
		{
			return (1);
		}
	}
	return (0);
}
