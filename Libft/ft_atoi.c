/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 08:25:12 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/01 10:57:48 by briveiro         ###   ########.fr       */
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
