/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:55:28 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:55:29 by pausanch         ###   ########.fr       */
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
