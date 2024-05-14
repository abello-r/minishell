/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:41:24 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/15 16:44:05 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void) argv;
	(void) envp;
	(void) argc;

	data_init(envp); // testingpepe
	data_init(envp); // testingalberto

	if (ft_loop(&data))
		return (1);
	else
		return (0);
}

// Compare the input line with the builtins to execute them
void fake_executor(t_data *data)
{
	if (ft_strlen(data->input) == 0) // If the input is empty,
		return ;

	if (ft_strncmp(data->input, "pwd", ft_strlen(data->input)) == 0)
		ft_pwd();
	else if (ft_strncmp(data->input, "env", ft_strlen(data->input)) == 0)
		ft_env();
	else {
		printf("%s: command not found\n", data->input);
	}
}

int	ft_loop(t_data *data)
{
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		data->input = readline("minishell$ ");
		if (!data->input || ft_pair_quotation_check(data))
			return (1);
		parser(data);
		fake_executor(data); // Debug de builtins
		free(data->input);

		// TODO: Implementar [ add_history ] luego de la comprobación de la linea
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

int	ft_pair_quotation_check(t_data *data)
{
	if (ft_character_counter(data->input, '\'') % 2 != 0)
	{
		printf("%s", SIMPLE_QUOTE);
		return (1);
	}
	else if (ft_character_counter(data->input, '\"') % 2 != 0)
	{
		printf("%s", DOUBLE_QUOTE);
		return (1);
	}
	return (0);
}
