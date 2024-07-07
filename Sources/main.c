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
	if (ft_strlen(data->input) == 0)
		return ;
	if (ft_strncmp(data->input, "pwd", ft_strlen(data->input)) == 0)
		ft_pwd();
    else if (ft_strncmp(data->input, "env", ft_strlen(data->input)) == 0)
        ft_env(data);
    else if (ft_strncmp(data->input, "unset", ft_strlen(data->input)) == 0)
        ft_unset(data, "COLORTERM="); // Change COLOR_TERM to the variable you want to unset
    else if (ft_strncmp(data->input, "export", ft_strlen(data->input)) == 0)
        ft_export(data);
	else if (ft_strncmp(data->input, "cd", ft_strlen(data->input)) == 0)
        ft_cd(data);
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
		// add_history(data->input);
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

void is_builtin(char *token_table, t_token *token)
{
	if (ft_strlen(token_table) == 0)
		return ;
	if (ft_strncmp(token_table, "pwd", ft_strlen(token_table)) == 0)
		token->type = "PWD";
	else if (ft_strncmp(token_table, "env", ft_strlen(token_table)) == 0)
		token->type = "ENV";
	else if (ft_strncmp(token_table, "unset", ft_strlen(token_table)) == 0)
		token->type = "UNSET";
	else if (ft_strncmp(token_table, "export", ft_strlen(token_table)) == 0)
		token->type = "EXPORT";
	else if (ft_strncmp(token_table, "cd", ft_strlen(token_table)) == 0)
		token->type = "CD";
}
