/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:54:57 by pausanch          #+#    #+#             */
/*   Updated: 2024/11/15 11:54:58 by pausanch         ###   ########.fr       */
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
