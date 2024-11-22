/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:46 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:56:47 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	tm;
	size_t	c;

	tm = 0;
	while (dst[tm] && tm < dstsize)
		tm++;
	c = tm;
	while (src[tm - c] && tm + 1 < dstsize)
	{
		dst[tm] = src[tm - c];
		tm++;
	}
	if (c < dstsize)
		dst[tm] = 0;
	return (ft_strlen(src) + c);
}
