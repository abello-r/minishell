/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 08:25:45 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/02 08:47:17 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	c1;
	size_t	c2;

	dest = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s1 || !s2 || !dest)
		return (0);
	c1 = -1;
	while (s1[++c1])
		dest[c1] = s1[c1];
	c2 = -1;
	while (s2[++c2])
		dest[c1 + c2] = s2[c2];
	dest[c1 + c2] = '\0';
	return (dest);
}
