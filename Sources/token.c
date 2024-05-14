/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:46:23 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/14 20:54:32 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

t_token *ft_token_new(int start, int count, char *input, t_data *data)
{
	data->token = NULL;
    t_token *new;
    char    *content;
    content = ft_substr(input, start, count - start);
    if (!ft_strcmp(content, "\"\"") || !ft_strcmp(content, "\'\'"))
    {
        free(content);
        return (NULL);
    }
    new = (t_token *)malloc(sizeof(t_token));
    if (!new)
        ft_print_exit("Malloc failed\n");
    //here
	return (new);
}
