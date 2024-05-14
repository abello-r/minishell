/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:59:04 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/14 20:10:37 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"
#include <limits.h>

void	ft_pwd(void)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, PATH_MAX) == NULL)
	{
		printf("Error: Isn't possible to get the current directory\n");
		return ;
	}
	printf("%s\n", pwd);
}
