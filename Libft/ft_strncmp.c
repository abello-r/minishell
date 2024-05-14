/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 10:50:05 by briveiro          #+#    #+#             */
/*   Updated: 2022/11/30 13:58:15 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	c;

	c = 0;
	if (n == 0)
		return (0);
	while (s1[c] == s2[c] && c < n - 1 && s1[c] != '\0' && s2[c] != '\0')
		c++;
	return ((unsigned char)s1[c] - (unsigned char)s2[c]);
}
