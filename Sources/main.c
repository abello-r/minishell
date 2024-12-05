/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:02:10 by briveiro          #+#    #+#             */
/*   Updated: 2024/12/05 15:09:20 by pausanch         ###   ########.fr       */
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
	ft_handle_redirections_and_pipes(data);
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
			continue;
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
	int i;

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
