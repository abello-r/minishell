/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:52:22 by abello-r          #+#    #+#             */
/*   Updated: 2024/07/21 12:43:39 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

static void	ft_print_echo_args(t_data *data, int dash_flag)
{
	int	tokens_count;

	tokens_count = ft_count_tokens(data) - 1;
	if (dash_flag == 1)
	{
		data->token = data->token->next;
		tokens_count -= 1;
	}
	while (data->token->next->content != NULL && \
		(ft_strcmp(data->token->next->type, "ARG") == 0 \
		|| ft_strcmp(data->token->next->type, "DQUOTE") == 0 \
		|| ft_strcmp(data->token->next->type, "SQUOTE") == 0 \
		|| ft_strcmp(data->token->next->type, "ENV") == 0))
	{
		if (tokens_count > 1)
			printf("%s ", data->token->next->content);
		else
			printf("%s", data->token->next->content);
		data->token = data->token->next;
		tokens_count--;
	}
	if (dash_flag == 0)
		printf("\n");
}

static int	check_dash_flag(t_data *data)
{
	int	i;

	i = 0;
	while (data->token->next->content[i] == '-')
	{
		if (data->token->next->content[i] == '-' && \
			data->token->next->content[i + 1] == 'n')
			return (1);
		i++;
	}
	return (0);
}

void	ft_echo(t_data *data)
{
	if (data->token->next->content == NULL)
		printf("\n");
	else
	{
		if (check_dash_flag(data))
			ft_print_echo_args(data, 1);
		else
			ft_print_echo_args(data, 0);
	}
}
