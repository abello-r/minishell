/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:31 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:56:32 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	char	*dest;
	size_t	count;

	s2 = (char *)s1;
	dest = (char *)malloc((ft_strlen(s2) + 1));
	count = -1;
	if (!dest)
		return (NULL);
	while (s2[++count])
		dest[count] = s2[count];
	dest[count] = '\0';
	return (dest);
}
