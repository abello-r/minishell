/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:55:33 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:55:34 by pausanch         ###   ########.fr       */
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
