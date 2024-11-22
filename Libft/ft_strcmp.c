/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:19 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:56:20 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = -1;
	if (ft_strlen(s1) != ft_strlen(s2))
		return (1);
	while (s1[++i] != '\0')
		if (s1[i] != s2[i])
			return (1);
	return (0);
}
