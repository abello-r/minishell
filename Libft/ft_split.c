/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: briveiro <briveiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 10:12:16 by briveiro          #+#    #+#             */
/*   Updated: 2022/12/06 17:13:29 by briveiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	contador(char const *s, char c);
static void		creador(char **cadena, char const *s, char c, size_t palabras);

char	**ft_split(char const *s, char c)
{
	char	**cadena;
	size_t	palabras;

	if (!s)
		return (NULL);
	palabras = contador(s, c);
	cadena = malloc(sizeof(char **) * (palabras + 1));
	if (!cadena)
		return (NULL);
	creador(cadena, s, c, palabras);
	return (cadena);
}

static size_t	contador(char const *s, char c)
{
	size_t	contadorpalablas;
	size_t	coincidencia;

	contadorpalablas = 0;
	coincidencia = 1;
	while (*s)
	{
		if (*s != c && coincidencia)
		{
			coincidencia = 0;
			contadorpalablas++;
		}
		else if (*s == c)
			coincidencia = 1;
		s++;
	}
	return (contadorpalablas);
}

static void	creador(char **cadena, char const *s, char c, size_t palabras)
{
	char	*cambio;

	while (*s && *s == c)
		s++;
	while (palabras--)
	{
		cambio = ft_strchr(s, c);
		if (cambio)
		{
			*cadena = ft_substr(s, 0, cambio - s);
			while (*cambio && *cambio == c)
				cambio++;
			s = cambio;
		}
		else
			*cadena = ft_substr(s, 0, ft_strlen(s) + 1);
		cadena++;
	}
	*cadena = NULL;
}
