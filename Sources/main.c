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
void fake_executor(t_data *data)
{
    if (ft_strlen(data->input) == 0) // If the input is empty,
        return ;
    if (ft_strncmp(data->input, "pwd", ft_strlen(data->input)) == 0)
        ft_pwd();
    else if (ft_strncmp(data->input, "env", ft_strlen(data->input)) == 0)
        ft_env(data);
    else if (ft_strncmp(data->input, "unset", ft_strlen(data->input)) == 0)
        ft_unset(data, "COLORTERM="); // Change COLOR_TERM to the variable you want to unset
    else if (ft_strncmp(data->input, "export", ft_strlen(data->input)) == 0)
        ft_export(data);
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
		data->input_len = ft_strlen(data->input);
		if (!data->input || ft_pair_quotation_check(data))
			return (1);

		parser(data);
		add_history(data->input);
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

int ft_redir_conditions_check(char *token)
{
	if (token[0] == '>')
	{
		if (token[1] == '>' && token[2] == '>')
			return (1);
		else if (token[1] == '>' && token[2] == '<')
			return (1);
		else if (token[1] == '<')
			return (1);
	}
	else if (token[0] == '<')
	{
		if (token[1] == '<' && token[2] == '<')
			return (1);
		else if (token[1] == '<' && token[2] == '>')
			return (1);
		else if (token[1] == '>')
			return (1);
	}
	return (0);
}

int	ft_redirection_check(char **token_table)
{
	int	a;

	a = 0;
	while (token_table[a])
	{
		if (token_table[a][0] != '\'' && token_table[a][0] != '\"')
		{
			if (ft_redir_conditions_check(token_table[a]))
				ft_print_exit(REDIR);
		}
		a++;
	}
	return (0);
}
