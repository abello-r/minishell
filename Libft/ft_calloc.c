/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:53:08 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:53:09 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*ret;

	ret = malloc(count * size);
	if (ret)
	{
		ft_bzero(ret, (count * size));
		return (ret);
	}
	else
		return (NULL);
}
