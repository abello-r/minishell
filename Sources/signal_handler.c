/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:45:14 by briveiro          #+#    #+#             */
/*   Updated: 2024/07/21 03:45:15 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	ft_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		write(1, "  \n", 3);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
