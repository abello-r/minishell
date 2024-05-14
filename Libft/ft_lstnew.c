/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 14:20:58 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/04 19:45:46 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*prueba;

	prueba = (t_list *)malloc(sizeof(t_list));
	if (!prueba)
		return (prueba = NULL);
	prueba->content = content;
	prueba->next = NULL;
	return (prueba);
}
