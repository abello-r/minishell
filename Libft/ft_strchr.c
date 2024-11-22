/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:14 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:56:15 by pausanch         ###   ########.fr       */
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
