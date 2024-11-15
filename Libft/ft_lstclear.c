/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:54:09 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:54:10 by pausanch         ###   ########.fr       */
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
