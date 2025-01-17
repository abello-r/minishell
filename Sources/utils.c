/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 03:46:15 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/13 15:51:48 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

char	*ft_strtolower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
	return (str);
}

void	ft_check_allocation(void *mem)
{
	if (!mem)
		ft_print_exit("Error: malloc failed\n");
}

t_token	*ft_add_node(char *content, char *type)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		ft_print_exit("Error: malloc failed\n");
	new_node->content = ft_strdup(content);
	if (!new_node->content)
	{
		free(new_node);
		ft_print_exit("Error: malloc failed\n");
	}
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	ft_utils_free_double_pointer(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

void	ft_utils_copy_double_pointer(char **src, char ***dst)
{
	int		array_len;
	int		i;
	char	**new_array;

	if (!src || !dst)
		return ;
	array_len = 0;
	while (src[array_len])
		array_len++;
	new_array = (char **)malloc(sizeof(char *) * (array_len + 1));
	if (!new_array)
		return ;
	i = 0;
	while (i < array_len)
	{
		new_array[i] = ft_strdup(src[i]);
		if (!new_array[i])
		{
			ft_utils_free_double_pointer(new_array);
			return ;
		}
		i++;
	}
	new_array[i] = NULL;
	*dst = new_array;
}
