/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 07:32:40 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/02 08:24:09 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*s2;
	size_t	count;

	if (!s)
		return (NULL);
	if ((size_t)start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > (ft_strlen(s) - start))
		len = (ft_strlen(s) - start);
	s2 = (char *)malloc(sizeof(char) * (len + 1));
	if (!s2)
		return (NULL);
	count = -1;
	while (++count < len)
		s2[count] = *(s + start + count);
	s2[count] = '\0';
	return (s2);
}
