/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:33:03 by abello-r          #+#    #+#             */
/*   Updated: 2024/12/12 16:17:54 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

/*
* Propósito: Verifica si una cadena es un identificador válido para ser una variable de entorno.
* Lógica:
	- Comprueba que el primer carácter sea una letra (isalpha) o un guion bajo (_).
	- Itera sobre el resto de los caracteres, asegurándose de que sean alfanuméricos o _.
	- Detiene la verificación si encuentra un carácter = (indicando el final del nombre de la variable).
*/
static int is_valid_identifier(const char *str)
{
    if (!str || (!ft_isalpha(*str) && *str != '_'))
        return 0;
    while (*str && *str != '=')
    {
        if (!ft_isalnum(*str) && *str != '_' )
            return 0;
        str++;
    }
    return 1;
}

/*
* Propósito: Libera la memoria de un arreglo de cadenas.
* Lógica:
	- Itera sobre el arreglo liberando cada cadena con free.
	- Finalmente, libera el arreglo en sí.
*/
static void ft_free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

/*
* Propósito: Hace una copia profunda del entorno actual (envp).
* Lógica:
	- Calcula el tamaño del entorno (envp).
	- Reserva memoria para el nuevo arreglo de cadenas.
	- Copia cada cadena de envp usando una función llamada ft_strdup.
	- Si ocurre un error durante la copia, libera la memoria ya asignada con ft_free_array.
*/
char **ft_copy_env(char **envp)
{
    int i;
    char **new_env;

    i = 0;
    while (envp[i])
        i++;
    new_env = malloc(sizeof(char *) * (i + 1));
    if (!new_env)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        new_env[i] = ft_strdup(envp[i]);
        if (!new_env[i])
        {
            ft_free_array(new_env);
            return (NULL);
        }
        i++;
    }
    new_env[i] = NULL;
    return (new_env);
}

/*
* Propósito: Imprime las variables de entorno en un formato específico.
* Formato de salida:
	declare -x VAR="value"
* Lógica:
	- Si una variable contiene =, divide el nombre de la variable y el valor, imprimiéndolos con el formato especificado.
	- Si no contiene =, imprime solo el nombre.
*/
static void print_formatted_env(char **env)
{
    int i;
    char *equals;

    for (i = 0; env[i]; i++)
    {
        equals = ft_strchr(env[i], '=');
        write(1, "declare -x ", 11);
        if (equals)
        {
            write(1, env[i], equals - env[i]);
            write(1, "=\"", 2);
            write(1, equals + 1, ft_strlen(equals + 1));
            write(1, "\"", 1);
        }
        else
        {
            write(1, env[i], ft_strlen(env[i]));
        }
        write(1, "\n", 1);
    }
}

/*
* Propósito: Añade o reemplaza una variable en el entorno.
* Lógica:
	- Extrae el nombre de la variable (parte antes de =).
	- Verifica si ya existe una variable con el mismo nombre.
		- Si existe, la reemplaza.
		- Si no existe, la añade al final.
	- Crea un nuevo arreglo de variables con el nuevo contenido y lo retorna.
*/
static char **ft_add_new_env(char **envp, char *d_new_env, int i)
{
	int repeated;
	char *key;
	char **new_envp;

	i = -1;
	repeated = 0;
	key = ft_substr(d_new_env, 0, ft_strchr(d_new_env, '=') - d_new_env);
	new_envp = malloc(sizeof(char *) * (ft_envp_len(envp) + 2));
	ft_check_allocation(new_envp);
	while (envp[++i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			repeated = 1;
			i++;
			free(envp[i]);
			new_envp[i] = ft_strdup(d_new_env);
		}
		else
			new_envp[i] = ft_strdup(envp[i]);
	}
	if (repeated == 0)
		new_envp[i] = ft_strdup(d_new_env);
	new_envp[i + 1] = NULL;
	return (new_envp);
}

/*
* Propósito: Procesa un argumento individual pasado a export.
* Lógica:
	- Valida que el argumento sea un identificador válido.
	- Si contiene =, añade o actualiza la variable en el entorno.
	- Si no contiene =, verifica si ya existe en el entorno:
		- Si no existe, la añade sin valor.
*/
void ft_args_export_iterator(t_data *data, char *arg)
{
    char *equals;
    char *var_name;

    if (!is_valid_identifier(arg))
    {
        ft_putstr_fd("minishell: export: `", 2);
        ft_putstr_fd(arg, 2);
        ft_putstr_fd("': not a valid identifier\n", 2);
        return;
    }
    equals = ft_strchr(arg, '=');
    if (equals)
    {
        var_name = ft_substr(arg, 0, equals - arg);
        data->envp = ft_add_new_env(data->envp, arg, 1);
        free(var_name);
    }
   	else
    {
		if (!arg)
			return;
		char *env_value = ft_get_env(data, arg);
        if (!env_value)
            data->envp = ft_add_new_env(data->envp, arg, 0);
    }
}

/*
* Propósito: Implementa el comportamiento general del comando export.
* Lógica:
	- Si no hay argumentos (argv[1] es NULL), imprime todas las variables de entorno en formato ordenado.
	- Si hay argumentos, procesa cada uno con ft_args_export_iterator
*/
void ft_export(t_data *data)
{
    if (!data->cmds->argv[1])
    {
        char **envp_copy = ft_copy_env(data->envp);
        print_formatted_env(envp_copy);
        ft_free_array(envp_copy);
        return;
    }

    int i = 1;
    while (data->cmds->argv[i])
    {
        ft_args_export_iterator(data, data->cmds->argv[i]);
        i++;
    }
}