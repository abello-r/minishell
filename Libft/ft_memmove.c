/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 11:07:39 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/04 19:14:11 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	c;
	size_t	coun;
	char	*a;
	char	*b;

	coun = -1;
	c = len;
	a = dst;
	b = (char *)src;
	if (!dst && !src)
		return (NULL);
	if (dst == src)
		return (dst);
	if (a > b)
	{
		while (c--)
			a[c] = b[c];
		return (dst);
	}
	while (++coun < len)
		a[coun] = b[coun];
	return (dst);
}
