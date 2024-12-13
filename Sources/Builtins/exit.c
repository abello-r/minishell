/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 11:58:43 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/13 16:44:13 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

/* Function to exit the current process:

- Exits with the arg if it exists and is numeric, otherwise exits with 0

- Returns error if there is more than 1 arg (does not exit)

- Returns error if there is a non-numeric arg (exits) */

void	ft_exit(t_data *data) // TO DO:
{
	(void)data;
	exit(0);
}
