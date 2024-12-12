/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 11:58:43 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/11 19:49:54 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

/* Function to exit the current process:

- Exits with the arg if it exists and is numeric, otherwise exits with 0

- Returns error if there is more than 1 arg (does not exit)

- Returns error if there is a non-numeric arg (exits) */

void	ft_exit(t_data *data) // TO DO:
{
	(void)data;
	exit(0);
}

/* int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

int	ft_atoi2(const char *nptr, long *nbr)
{
	int		sign;

	sign = 1;
	*nbr = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
		sign = -sign;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	if (!ft_isdigit(*nptr))
		return (-1);
	while (ft_isdigit(*nptr))
	{
		*nbr = 10 * *nbr + (*nptr - '0');
		nptr++;
	}
	if (*nptr && !ft_isspace(*nptr))
		return (-1);
	*nbr *= sign;
	return (0);
} */

/* int ft_exit(t_data *data)
{
    t_cmd *node = data->cmds;  // Obtener el primer comando de la lista
    long status[2];

    // Si no hay argumentos, simplemente salimos con código 0
    if (!node->argv || !node->argv[1])
    {
        ft_putstr_fd("exit\n", 2);  // Imprimir "exit" solo si es el último comando
        exit(0);  // Salimos con código de éxito
    }

    // Intentamos convertir el argumento a un número
    status[1] = ft_atoi2(node->argv[1], &status[0]);

    // Si no es un número válido
    if (status[1] == -1)
    {
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(node->argv[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(255);  // Salimos con código de error 255
    }
    
    // Si hay más de un argumento, mostramos un error
    if (node->argv[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        return 1;  // No salimos, solo indicamos error
    }

    // Si hay un solo argumento, salimos con ese código (y lo normalizamos entre 0 y 255)
    status[0] %= 256 + 256 * (status[0] < 0);
    ft_putstr_fd("exit\n", 2);  // Imprimir "exit" solo si es el último comando
    exit(status[0]);  // Salimos con el código de salida calculado
} */