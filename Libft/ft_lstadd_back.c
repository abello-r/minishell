/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 14:53:59 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/05 15:04:40 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*nuevalista;

	nuevalista = ft_lstlast(*lst);
	if (nuevalista)
		nuevalista->next = new;
	else
		*lst = new;
}
