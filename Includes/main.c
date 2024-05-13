#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	ft_loop(&data);

	return (0);
}

void	ft_loop(t_data *data)
{
	while (1)
	{
		data->input = readline("minishell$ ");
		if (!data->input)
			print_exit("exit\n");
		if (!ft_pair_quotation_check(data))
			print_exit("Error: quotation not closed\n");
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
	int	count;

	if ((ft_is_quotation_closed(data->input, '\'') % 2 != 0)
		|| (ft_is_quotation_closed(data->input, '\"') % 2 != 0))
		return (0);
	return (1);
	// Añadir variable len
}
