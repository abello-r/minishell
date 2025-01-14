/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:41:19 by pausanch          #+#    #+#             */
/*   Updated: 2025/01/14 17:24:57 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

char	*validate_directory(char *dir_path, t_data *data)
{
	if (ft_strncmp(dir_path, "OLDPWD", 6) == 0)
	{
		dir_path = ft_get_env(data, "OLDPWD");
		if (!dir_path)
			return (NULL);
	}
	if (access(dir_path, F_OK) != 0)
	{
		perror("");
		g_status = 1;
		return (NULL);
	}
	return (dir_path);
}
