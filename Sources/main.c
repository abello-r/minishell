#include "../Includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void) argv;
	(void) envp;
	(void) argc;

	if (ft_loop(&data))
		return (1);
	else
		return (0);
}

int	ft_loop(t_data *data)
{
	while (1)
	{
		signal(SIGINT, ft_signal_handler);
		data->input = readline("minishell$ ");
		if (!data->input || ft_pair_quotation_check(data))
			return (1);

		printf("%s\n", data->input);
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
