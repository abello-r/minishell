int ft_isspace(int c)
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
}

void ft_exit(t_data *data, int last_status)
{
    t_cmd *node;  // Obtener el primer comando de la lista
    long status[2];

	node = data->cmds;
    // Si no hay argumentos, simplemente salimos con código 0
    if (!node->argv || !node->argv[1])
    {
        ft_putstr_fd("exit\n", 2);  // Imprimir "exit" solo si es el último comando
        exit(last_status);  // Salimos con código de éxito
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
        return ;  // No salimos, solo indicamos error
    }

    // Si hay un solo argumento, salimos con ese código (y lo normalizamos entre 0 y 255)
    status[0] %= 256 + 256 * (status[0] < 0);
    exit(status[0]);  // Salimos con el código de salida calculado
}