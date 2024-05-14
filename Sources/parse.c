/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:41:56 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/14 20:49:44 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void    parser(t_data *data)
{
    int count;
    // int  start;
    count = 0;
    while (data->input[count] == ' ')
        count++;
    // if (ft_strchr("'\"''\''", data->input[count]))
    // {
    //  // to do
    // }
    // start = count;
    // else if (ft_strchr("|><", data->input[count]))
    // {
    //  ft_redirect_parse(data); // doing
    // }
}

// void ft_redirect_parse(t_data *data, char c, int start, int count)
// {
// }

void    ft_token_add_back(t_token **token, t_token *new)
{
    t_token *last;
    if (!new)
        return ;
    if (!*token)
    {
        *token = new;
        return ;
    }
    last = ft_token_last(*token);
    last->next = new;
    new->prev = last;
}

t_token *ft_token_last(t_token *token)
{
    if (!token)
        return (NULL);
    while (token->next)
        token = token->next;
    return (token);
}
