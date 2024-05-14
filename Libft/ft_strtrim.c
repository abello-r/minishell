/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 08:48:49 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/02 09:38:01 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	count;

	if (!s1 || !set)
		return (0);
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	count = ft_strlen(s1);
	while (count && ft_strchr(set, s1[count]))
		count--;
	return (ft_substr(s1, 0, count + 1));
}
