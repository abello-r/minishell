/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:40:13 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/03 19:15:04 by briveiro         ###   ########.fr       */
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
