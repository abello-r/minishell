/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 04:02:10 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/21 04:08:50 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void) argv;
	(void) envp;
	(void) argc;

	initializer(envp, &data);

	if (ft_loop(&data))
		return (1);
	else
		return (0);
}

// Compare the input line with the builtins to execute them
void	fake_executor(t_data *data)
{
	/*
		DEBUG
		while(data->token->content != NULL) {
			printf("Input: %s\n", data->token->content);
			if (ft_strlen(data->token->content) == 0)
				return ;
			data->token = data->token->next;
	}*/

	if (ft_strlen(data->input) == 0)
		return ;
	if (ft_strncmp(data->token->content, "pwd", ft_strlen("pwd")) == 0)
		ft_pwd(); // OK
    else if (ft_strncmp(data->token->content, "env", ft_strlen("env")) == 0)
        ft_env(data); // OK
    else if (ft_strncmp(data->token->content, "unset", ft_strlen("unset")) == 0)
        ft_unset(data); // TODO: Fix unset c
    else if (ft_strncmp(data->token->content, "export", ft_strlen("export")) == 0)
        ft_export(data); // ARREGLADO a=1 b=2 c=3
	else if (ft_strncmp(data->token->content, "cd", ft_strlen("cd")) == 0)
        ft_cd(data); // OK
	else if (ft_strncmp(data->token->content, "echo", ft_strlen("echo")) == 0)
        ft_echo(data); // TODO: Fix $USER
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
		data->input_len = ft_strlen(data->input);
		if (!data->input || ft_pair_quotation_check(data))
			return (1);
		parser(data);
		add_history(data->input);
		fake_executor(data);
		free(data->input);
	}
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

int	is_builtin(char *token_table, t_token *token)
{
	char	*temp;

	temp = ft_calloc(100, sizeof(char));
	temp = ft_strcpy(temp, token_table);
	temp = ft_strtolower(temp);
	if (ft_strncmp(temp, "pwd", ft_strlen(temp)) == 0 \
		|| ft_strncmp(temp, "env", ft_strlen(temp)) == 0 \
		|| ft_strncmp(temp, "unset", ft_strlen(temp)) == 0 \
		|| ft_strncmp(temp, "export", ft_strlen(temp)) == 0 \
		|| ft_strncmp(temp, "cd", ft_strlen(temp)) == 0 \
		|| ft_strncmp(temp, "echo", ft_strlen(temp)) == 0)
	{
		token->type = "BUILTIN";
		return (1);
	}
	return (0);
}
