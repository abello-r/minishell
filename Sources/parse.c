#include "../Includes/minishell.h"

void	parser(t_data *data)
{
	int	count;

	count = 0;
	while (data->input[count] == ' ')
		count++;
	if (ft_strchr("'\"''\''", data->input[count]))
	{
		
	}
}

// void	check_all_quotes(t_data *data)
// {
	
// }