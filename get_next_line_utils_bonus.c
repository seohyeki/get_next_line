/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seohyeki <seohyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:05:56 by seohyeki          #+#    #+#             */
/*   Updated: 2023/11/01 23:20:30 by seohyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_find_nl(const char *s, t_data *data)
{
	data->len = 0;
	while (data->line[data->len] != '\0')
		data->len++;
	data->pos = 0;
	while (s[data->pos] != '\0')
	{
		if (s[data->pos] == '\n')
			return (1);
		data->pos++;
	}
	return (0);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*dest;

	i = 0;
	if (s1 == NULL)
	{
		dest = (char *)malloc(sizeof(char));
		if (dest == NULL)
			return (NULL);
		dest[0] = '\0';
		return (dest);	
	}
	dest = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (dest == 0)
		return (NULL);
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_re_malloc(char *origin, int size)
{
	char	*new;
	int		i;

	new = (char *)malloc(sizeof(char) * (size + 1));
	if (new == 0)
  {
    free(origin);
		return (0);
  }
  new[size] = '\0';
	i = 0;
	while (origin[i] != '\0')
	{
		new[i] = origin[i];
		i++;
	}
	new[i] = '\0';
	free(origin);
	return (new);
}

t_list	*add_newnode(int fd)
{
	t_list	*newnode;

	newnode = (t_list *)malloc(sizeof(t_list));
	if (newnode == NULL)
		return (NULL);
	newnode->fd = fd;
	newnode->content = 0;
	newnode->next = 0;
	return (newnode);
}