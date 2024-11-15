/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:53:24 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:53:26 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	long	cont;
	long	pos;
	int		neg;

	cont = 0;
	pos = 0;
	neg = 0;
	while ((str[pos] <= 13 && str[pos] >= 9) || str[pos] == 32)
		pos++;
	if (str[pos] && str[pos] == 45)
	{
		neg = 1;
		pos++;
	}
	else if (str[pos] && str[pos] == '+')
		pos++;
	while (ft_isdigit(str[pos]) && str[pos])
	{
		cont = (cont * 10) + (str[pos] - '0');
		pos++;
	}
	if (neg == 1)
		return (-cont);
	return (cont);
}
