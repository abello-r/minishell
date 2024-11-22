/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:53:50 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:53:51 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	nlen(long n)
{
	size_t	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n = n * -1;
	}
	while (n > 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	long	ernumero;
	size_t	cifras;
	char	*copia;

	ernumero = n;
	cifras = nlen(ernumero);
	copia = (char *)malloc(sizeof(char) * (cifras + 1));
	if (!copia)
		return (0);
	copia[cifras] = '\0';
	if (n == 0)
		copia[0] = '0';
	if (ernumero < 0)
	{
		ernumero = ernumero * -1;
		copia[0] = '-';
	}
	while (ernumero > 0)
	{
		cifras--;
		copia[cifras] = (ernumero % 10) + '0';
		ernumero = (ernumero / 10);
	}
	return (copia);
}
