/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:02:10 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/13 15:55:39 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define C "\x1b[36m"
#define Y "\x1b[33m"
#define R "\e[0m"
#include "../Includes/minishell.h"

int	g_status = 0;

void	ft_aux_cleanup(t_data *data)
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
}

void	cleanup_data(t_data *data)
{
	t_token	*current;
	t_token	*next;

	if (!data)
		return ;
	if (data->head)
	{
		current = data->head;
		while (current)
		{
			next = current->next;
			if (current->content && current->content != (char *)-1)
				free(current->content);
			free(current);
			current = next;
		}
		data->head = NULL;
		data->token = NULL;
	}
	ft_aux_cleanup(data);
}

int	main(int argc, char **argv, char **envp)
{
	int		ret;
	t_data	data;

	(void)argv;
	(void)argc;
	initializer(envp, &data);
	ret = ft_loop(&data);
	if (ret)
		return (1);
	else
		return (0);
}

void	ft_control_input(t_data *data)
{
	printf("\033[Fminishell$ exit\n");
	cleanup_data(data);
	exit(EXIT_FAILURE);
}

int	ft_loop(t_data *data)
{
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		data->input = readline(Y "M" C "inishell" "\x1b[33mツ " R);
		if (!data->input)
			ft_control_input(data);
		if (ft_pair_quotation_check(data))
		{
			free(data->input);
			continue ;
		}
		if (ft_is_empty(data->input) != 1)
		{
			add_history(data->input);
			parser(data);
			data->cmds = parse_tokens_to_commands(data->token);
			ft_execute_commands(data);
			ft_free_commands(data->cmds);
			ft_free_tokens(data->head);
		}
		free(data->input);
		data->input = NULL;
	}
	return (0);
}
