#include "../Includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	// printf("'\"''\''");
	(void) argv;
	(void) envp;
	(void) argc;
	ft_loop(&data);

	return (0);
}

void	ft_loop(t_data *data)
{
	while (1)
	{
		data->input = readline("minishell$ ");
		if (!data->input)
			ft_print_exit("exit\n");
		if (!ft_pair_quotation_check(data))
			ft_print_exit(SIMPLE_QUOTE);
		printf("%s\n", data->input);
		free(data->input);
		// implementar add_history al final del checker
	}
}

int	ft_is_quotation_closed(char *input, char c)
{
	int	count;
	int	total;

	count = 0;
	total = 0;

	while (input[count])
	{
		if (input[count] == c)
			total++;
		count++;
	}
	return (total);
}

int	ft_pair_quotation_check(t_data *data)
{
	// int	count;
	// return 0 o return con codigo de salida de error

	if ((ft_is_quotation_closed(data->input, '\'') % 2 != 0)
		|| (ft_is_quotation_closed(data->input, '\"') % 2 != 0))
		return (0);
	if (ft_strchr("<>|;", data->input[ft_strlen(data->input) - 1]))
		return (0);
	return (1);
	// Añadir variable len
}
