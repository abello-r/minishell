/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:57:33 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/12 18:44:55 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void print_commands(t_cmd *commands)
{
	t_cmd *current = commands;

	if (!current) {
		printf("The command list is empty.\n");
		return;
	}

	while (current)
	{
		if (current->argv && current->argv[0]) {
			printf("\n");
			printf("Command: %s\n", current->argv[0]);
			printf("Arguments: %s\n", current->argv[1]);
			printf("Input file: %s\n", current->input_file);
			printf("Output file: %s\n", current->output_file);
			printf("Append: %d\n", current->append);
		} else {
			printf("Command: (no arguments or invalid structure)\n");
		}
		current = current->next;
	}
}

t_token *skip_to_next_command(t_token *tokens)
{
	while (tokens)
	{
		if (strcmp(tokens->type, "PIPE") == 0)
			return tokens->next;
		tokens = tokens->next;
	}
	return NULL;
}

int check_append(t_token *tokens)
{
	while (tokens && strcmp(tokens->type, "PIPE") != 0)
	{
		if (strcmp(tokens->type, "APPEND") == 0)
			return 1;
		tokens = tokens->next;
	}
	return 0;
}
/*
	@return: Files after '>' o '>>'
*/
char *extract_output_file(t_token *tokens)
{
	while (tokens && strcmp(tokens->type, "PIPE") != 0)
	{
		if (strcmp(tokens->type, "OUT") == 0 || strcmp(tokens->type, "APPEND") == 0)
		{
			if (tokens->next && strcmp(tokens->next->type, "ARG") == 0)
				return strdup(tokens->next->content);
		}
		tokens = tokens->next;
	}
	return NULL;
}

/*
	@return: Files after '<'
*/
char *extract_input_file(t_token *tokens)
{
	while (tokens && strcmp(tokens->type, "PIPE") != 0)
	{
		if (strcmp(tokens->type, "INPUT") == 0)
		{
			if (tokens->next && strcmp(tokens->next->type, "ARG") == 0)
				return strdup(tokens->next->content);
		}
		tokens = tokens->next;
	}
	return NULL;
}

char **extract_arguments(t_token *tokens)
{
	int	count;
	int	i;
	t_token *temp = tokens;

	count = 0;
	i = 0;
	while (temp && strcmp(temp->type, "PIPE") != 0)
	{
		if (strcmp(temp->type, "ARG") == 0 || strcmp(temp->type, "BUILTIN") == 0
			|| strcmp(temp->type, "DQUOTE") == 0 || strcmp(temp->type, "SQUOTE") == 0
			|| strcmp(temp->type, "ENV") == 0)
			count++;
		temp = temp->next;
	}
	char **argv = malloc((count + 1) * sizeof(char *));
	if (!argv)
		return NULL;
	while (tokens && strcmp(tokens->type, "PIPE") != 0)
	{
		if (strcmp(tokens->type, "ARG") == 0 || strcmp(tokens->type, "BUILTIN") == 0
			|| strcmp(tokens->type, "DQUOTE") == 0 || strcmp(tokens->type, "SQUOTE") == 0
			|| strcmp(tokens->type, "ENV") == 0)
			argv[i++] = strdup(tokens->content);
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return argv;
}

t_cmd *parse_tokens_to_commands(t_token *tokens)
{
	t_cmd	*head = NULL;
	t_cmd *current = NULL;

	while (tokens)
	{
		t_cmd *new_cmd = malloc(sizeof(t_cmd));
		if (!new_cmd)
			return NULL;
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
