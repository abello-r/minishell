/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 06:41:53 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/04 19:24:05 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	p;
	int	lensss ;

	lensss = ft_strlen(s);
	p = -1;
	if ((char )c == 0)
		return (((char *)s + lensss));
	while (s[++p])
	{
		if (s[p] == (char)c)
			return (((char *)s + p));
	}
	return (0);
}
