/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:32:03 by briveiro          #+#    #+#             */
/*   Updated: 2024/05/13 17:22:57 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// TODO: Please check if this shit work

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*nueval;
	t_list	*nuevac;

	if (!lst)
		return (NULL);
	nueval = 0;
	while (lst)
	{
		nuevac = ft_lstnew(f(lst->content));
		if (!nuevac)
		{
			ft_lstclear(&nueval, del);
			return (NULL);
		}
		ft_lstadd_back(&nueval, nuevac);
		lst = lst->next;
	}
	return (nueval);
}
