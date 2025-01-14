/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:57:33 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 17:27:54 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

t_token	*skip_to_next_command(t_token *tokens)
{
	while (tokens)
	{
		if (ft_strcmp(tokens->type, "PIPE") == 0)
			return (tokens->next);
		tokens = tokens->next;
	}
	return (NULL);
}

int	check_append(t_token *tokens)
{
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "APPEND") == 0)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

char	*extract_output_file(t_token *tokens)
{
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "OUT") == 0 \
			|| ft_strcmp(tokens->type, "APPEND") == 0)
		{
			if (tokens->next && ft_strcmp(tokens->next->type, "ARG") == 0)
				return (ft_strdup(tokens->next->content));
		}
		tokens = tokens->next;
	}
	return (NULL);
}

char	*extract_input_file(t_token *tokens)
{
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "INPUT") == 0)
		{
			if (tokens->next && ft_strcmp(tokens->next->type, "ARG") == 0)
				return (ft_strdup(tokens->next->content));
		}
		tokens = tokens->next;
	}
	return (NULL);
}

char	**extract_arguments(t_token *tokens)
{
	int		count;
	int		i;
	t_token	*temp;
	char	**argv;

	temp = tokens;
	count = 0;
	i = 0;
	while (temp && ft_strcmp(temp->type, "PIPE") != 0)
	{
		if (ft_strcmp(temp->type, "ARG") == 0 || ft_strcmp(temp->type, "BUILTIN") == 0
			|| ft_strcmp(temp->type, "DQUOTE") == 0 || ft_strcmp(temp->type, "SQUOTE") == 0
			|| ft_strcmp(temp->type, "ENV") == 0 || ft_strcmp(temp->type, "HEREDOC") == 0)
			count++;
		temp = temp->next;
	}
	argv = malloc((count + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	while (tokens && ft_strcmp(tokens->type, "PIPE") != 0)
	{
		if (ft_strcmp(tokens->type, "ARG") == 0 || ft_strcmp(tokens->type, "BUILTIN") == 0
			|| ft_strcmp(tokens->type, "DQUOTE") == 0 || ft_strcmp(tokens->type, "SQUOTE") == 0
			|| ft_strcmp(tokens->type, "ENV") == 0 || ft_strcmp(tokens->type, "HEREDOC") == 0)
			argv[i++] = ft_strdup(tokens->content);
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}

t_cmd	*parse_tokens_to_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_cmd	*new_cmd;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		new_cmd = malloc(sizeof(t_cmd));
		if (!new_cmd)
			return (NULL);
		new_cmd->argv = extract_arguments(tokens);
		new_cmd->input_file = extract_input_file(tokens);
		new_cmd->output_file = extract_output_file(tokens);
		new_cmd->append = check_append(tokens);
		new_cmd->next = NULL;
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		tokens = skip_to_next_command(tokens);
	}
	return (head);
}
