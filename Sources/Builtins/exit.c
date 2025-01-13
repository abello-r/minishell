/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:34:46 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/13 11:40:16 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

static int	ft_atoi2(const char *nptr, long *nbr)
{
	int	sign;

	sign = 1;
	*nbr = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
		sign = -sign;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	if (!ft_isdigit(*nptr))
		return (-1);
	while (ft_isdigit(*nptr))
	{
		*nbr = 10 * *nbr + (*nptr - '0');
		nptr++;
	}
	if (*nptr && !ft_isspace(*nptr))
		return (-1);
	*nbr *= sign;
	return (0);
}

static void	handle_special_status(long status)
{
	if (status == 126)
	{
		ft_putstr_fd("minishell: permission denied\n", 2);
		exit(126);
	}
	else if (status == 127)
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		exit(127);
	}
	exit((unsigned char)status);
}

static void	handle_numeric_arg(t_cmd *node, long status[2])
{
	status[1] = ft_atoi2(node->argv[1], &status[0]);
	if (status[1] == -1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		if (node->argv[1])
			ft_putstr_fd(node->argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
}

void	ft_exit(t_data *data)
{
	t_cmd	*node;
	long	status[2];

	if (!data || !data->cmds)
    {
        if (data)
            cleanup_data(data);
        exit(g_status);
    }

	node = data->cmds;
	ft_putstr_fd("exit\n", 2);
	if (!node->argv || !node->argv[1])
	{
		cleanup_data(data);
		exit(g_status);
	}
	handle_numeric_arg(node, status);
	if (node->argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_status = 1;
		return ;
	}
	cleanup_data(data);
	handle_special_status(status[0]);
}
