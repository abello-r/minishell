/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:02:10 by briveiro          #+#    #+#             */
/*   Updated: 2024/12/10 17:27:28 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void) argv;
	(void) argc;

	initializer(envp, &data);

	if (ft_loop(&data))
		return (1);
	else
		return (0);
}

void	fake_executor(t_data *data)
{
	if (ft_strlen(data->input) == 0)
		return ;
	ft_execute_commands(data);
}

int	ft_loop(t_data *data)
{
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		data->input = readline("minishell$ ");
		if (!data->input)
		{
			printf("\033[Fminishell$ exit\n");
			break; // TODO: change to exit (g_status + signal) var later
		}
		data->input_len = ft_strlen(data->input);
		if (ft_pair_quotation_check(data))
		{
			free(data->input);
			continue ;
		}
		parser(data);
		add_history(data->input);
		data->cmds = parse_tokens_to_commands(data->token);
		fake_executor(data);
		free(data->input);
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
