/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 06:30:51 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/02 07:33:06 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	c;
	char	*a;
	char	*b;

	c = 0;
	a = (char *)dst;
	b = (char *)src;
	if (n == 0 || dst == src)
	{
		return (dst);
	}
	while (c < n)
	{
		a[c] = b[c];
		c++;
	}
	return (dst);
}
