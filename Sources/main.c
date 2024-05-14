#include "../Includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void) argv;
	(void) envp;
	(void) argc;

	if (ft_loop(&data) == 1)
		return (1);
	else
		return (0);
}

int	ft_loop(t_data *data)
{
	while (1)
	{
		data->input = readline("minishell$ ");
		if (!data->input || !ft_pair_quotation_check(data))
			return (1);
		else
		{
			printf("%s\n", data->input);
			free(data->input);
		}
		// TODO: Implementar [ add_history ] luego de la comprobación de la linea
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
	if ((ft_is_quotation_closed(data->input, '\'') % 2 != 0) || (ft_is_quotation_closed(data->input, '\"') % 2 != 0))
		return (0);
	if (ft_strchr("<>|;", data->input[ft_strlen(data->input) - 1]))
		return (0);
	return (1);
}
