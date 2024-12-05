/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:57:33 by pausanch          #+#    #+#             */
/*   Updated: 2024/12/05 15:08:40 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

#include <stdio.h>

// Función para mostrar los datos de t_cmd
void print_commands(t_cmd *commands)
{
    t_cmd *current = commands;
    int cmd_num = 1;

    while (current)
    {
        printf("Comando %d:\n", cmd_num);

        // Mostrar argumentos
        printf("  argv: ");
        for (char **arg = current->argv; arg && *arg; ++arg)
        {
            printf("%s ", *arg);
        }
        printf("\n");

        // Mostrar archivo de entrada
        if (current->input_file)
            printf("  Archivo de entrada: %s\n", current->input_file);
        else
            printf("  Archivo de entrada: (ninguno)\n");

        // Mostrar archivo de salida
        if (current->output_file)
            printf("  Archivo de salida: %s\n", current->output_file);
        else
            printf("  Archivo de salida: (ninguno)\n");

        // Mostrar si la salida es en modo append
        printf("  Append: %s\n", current->append ? "Sí" : "No");

        // Avanzar al siguiente comando
        current = current->next;
        cmd_num++;
    }
}


t_token *skip_to_next_command(t_token *tokens)
{
    while (tokens)
    {
        if (strcmp(tokens->type, "PIPE") == 0)
            return tokens->next; // Saltar el pipe y avanzar
        tokens = tokens->next;
    }
    return NULL; // Fin de los tokens
}


int check_append(t_token *tokens)
{
    while (tokens && strcmp(tokens->type, "PIPE") != 0)
    {
        if (strcmp(tokens->type, "APPEND") == 0)
            return 1; // Modo append encontrado
        tokens = tokens->next;
    }
    return 0; // No hay append
}

char *extract_output_file(t_token *tokens)
{
    while (tokens && strcmp(tokens->type, "PIPE") != 0)
    {
        if (strcmp(tokens->type, "REDIRECT_OUT") == 0 || strcmp(tokens->type, "APPEND") == 0)
        {
            if (tokens->next && strcmp(tokens->next->type, "ARG") == 0)
                return strdup(tokens->next->content); // Archivo después de '>' o '>>'
        }
        tokens = tokens->next;
    }
    return NULL; // No hay redirección de salida
}

char *extract_input_file(t_token *tokens)
{
    while (tokens && strcmp(tokens->type, "PIPE") != 0)
    {
        if (strcmp(tokens->type, "REDIRECT_IN") == 0)
        {
            if (tokens->next && strcmp(tokens->next->type, "ARG") == 0)
                return strdup(tokens->next->content); // Archivo después de '<'
        }
        tokens = tokens->next;
    }
    return NULL; // No hay redirección de entrada
}

char **extract_arguments(t_token *tokens)
{
    int count = 0;
    t_token *temp = tokens;

    // Contar los argumentos del comando actual (sin operadores)
    while (temp && strcmp(temp->type, "PIPE") != 0)
    {
        if (strcmp(temp->type, "ARG") == 0)
            count++;
        temp = temp->next;
    }

    // Reservar memoria para los argumentos
    char **argv = malloc((count + 1) * sizeof(char *));
    if (!argv)
        return NULL;
    // Llenar el array de argumentos
    int i = 0;
    while (tokens && strcmp(tokens->type, "PIPE") != 0)
    {
        if (strcmp(tokens->type, "ARG") == 0)
            argv[i++] = strdup(tokens->content);
        tokens = tokens->next;
    }
    argv[i] = NULL; // Terminar el array con NULL
    return argv;
}

t_cmd *parse_tokens_to_commands(t_token *tokens)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;

    while (tokens)
    {
        t_cmd *new_cmd = malloc(sizeof(t_cmd));
        new_cmd->argv = extract_arguments(tokens);
        new_cmd->input_file = extract_input_file(tokens);
        new_cmd->output_file = extract_output_file(tokens);
        new_cmd->append = check_append(tokens);
        new_cmd->next = NULL;

        if (!head)
            head = new_cmd;
        else
            current->next = new_cmd;

        current = new_cmd;

        tokens = skip_to_next_command(tokens); // Salta al próximo bloque de tokens.
    }

    return head;
}
