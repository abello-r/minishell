/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 02:52:22 by abello-r          #+#    #+#             */
/*   Updated: 2025/01/14 16:27:37 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

extern int	g_status;

char	*ft_strncmp_turbo(const char *s1, const char *s2, size_t n)
{
	size_t	c;

	c = 0;
	if (n == 0)
		return (NULL);
	while (s1[c] && s2[c] && s1[c] == s2[c] && s1[c] != '=' && s2[c] != '=')
		c++;
	if (s2[c] == '\0' && s1[c] == '=')
		return (ft_strdup(s1 + c + 1));
	return (NULL);
}

static size_t	ft_len(const char *s)
{
	size_t	count;

	if (!s)
		return (0);
	count = 0;
	while (s[count] != 0)
	{
		count++;
	}
	return (count);
}

char	*ft_replace_content(char *s, char *obj, char *c)
{
	char	*r;
	int		i;
	int		j;
	int		k;

	if (!s || !obj || !c)
		return (NULL);
	r = (char *)malloc(sizeof(char) * ft_len(s) - ft_len(obj) + ft_len(c) + 1);
	ft_check_allocation(r);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (ft_strncmp(&s[i], obj, ft_strlen(obj)) == 0)
		{
			k = 0;
			while (c[k])
				r[j++] = c[k++];
			i += ft_strlen(obj);
		}
		else
			r[j++] = s[i++];
	}
	r[j] = '\0';
	return (r);
}

static int	check_dash_flag(t_data *data)
{
	int	i;

	i = 0;
	if (ft_strcmp(data->token->next->type, "ARG") != 0)
		return (0);
	while (data->token->next->content[i] == '-')
	{
		if (data->token->next->content[i] == '-'
			&& data->token->next->content[i + 1] == 'n')
			return (1);
		i++;
	}
	return (0);
}

void	ft_echo(t_data *data)
{
	data->token = data->head;
	if (data->token->next == NULL)
		printf("\n");
	else
	{
		if (check_dash_flag(data))
			ft_print_echo_args(data, 1);
		else
			ft_print_echo_args(data, 0);
	}
}
