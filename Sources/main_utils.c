#include "../Includes/minishell.h"

void	ft_free_commands(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current)
	{
		next = current->next;
		if (current->argv)
			ft_utils_free_double_pointer(current->argv);
		if (current->input_file)
			free(current->input_file);
		if (current->output_file)
			free(current->output_file);
		free(current);
		current = next;
	}
}

void	ft_free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
}

int	is_builtin(char *content)
{
	int	i;

	i = ft_strlen(content);
	if ((content[0] == 'c' && ft_strlen(content) == 2)
		|| ft_strlen(content) >= 3)
	{
		if (ft_strncmp(content, "pwd", i) == 0
			|| ft_strncmp(content, "env", i) == 0
			|| ft_strncmp(content, "unset", i) == 0
			|| ft_strncmp(content, "export", i) == 0
			|| ft_strncmp(content, "cd", i) == 0
			|| ft_strncmp(content, "echo", i) == 0
			|| ft_strncmp(content, "exit", i) == 0)
		{
			return (1);
		}
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
