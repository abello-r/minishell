/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abello-r <abello-r@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:50:09 by abello-r          #+#    #+#             */
/*   Updated: 2024/05/13 17:40:39 by abello-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	char *line = readline("Ingresa una línea de texto: ");
	printf("Línea ingresada: %s\n", line);

	return (0);
}
