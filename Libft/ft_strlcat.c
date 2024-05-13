/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 17:29:49 by briveiro          #+#    #+#             */
/*   Updated: 2022/11/30 08:31:35 by briveiro         ###   ########.fr       */
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
