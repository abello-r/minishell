/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 18:30:26 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/06 18:23:30 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*ptnodo;
	t_list	*elimn;

	ptnodo = *lst;
	while (ptnodo)
	{
		elimn = ptnodo->next;
		ft_lstdelone(ptnodo, del);
		ptnodo = elimn;
	}
	*lst = 0;
}
