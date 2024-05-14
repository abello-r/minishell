/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:52:22 by briveiro          #+#    #+#             */
/*   Updated: 2022/11/29 05:35:59 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
// B es hola
// C es x
// len es 3
//
void	*ft_memset(void *b, int c, size_t len)
{
	size_t	x;

	x = 0;
	while (x < len)
	{
		((unsigned char *)b)[x] = (unsigned char)c;
		x++;
	}
	return (b);
}

// ((type)arr)[i] == *((type)arr + i)