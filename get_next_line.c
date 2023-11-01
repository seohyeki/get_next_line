/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seohyeki <seohyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 20:35:56 by seohyeki          #+#    #+#             */
/*   Updated: 2023/11/01 10:57:38 by seohyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	chk_backup(t_list **fdlst, t_data *data, int fd)
{
	t_list	*previous;

	while (*fdlst != 0)
	{
		if ((*fdlst)->fd == fd)
		{
			data->line = ft_strdup((*fdlst)->content);
			if (data->line == 0)
				return (0);
			return (1);
		}
		previous = *fdlst;
		*fdlst = (*fdlst)->next;
	}
	if ((*fdlst) == 0)
		*fdlst = add_newnode(fd);
	else
		previous->next = add_newnode(fd);
	data->line = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (data->line == 0)
		return (0);
	data->line[0] = '\0';
	return (1);
}

static void	read_file(t_data *data, int fd)
{
	int	n;

	n = 1;
	data->buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (data->buf == NULL)
		return ;
	data->buf[BUFFER_SIZE] = '\0';
	while (ft_find_nl(data->line, data) == 0)
	{
		data->read_size = read(fd, data->buf, BUFFER_SIZE);
		if (data->read_size <= 0)
			return ;
		data->buf[data->read_size] = '\0';
		if (data->len + data->read_size >= BUFFER_SIZE * n)
		{
			n *= 2;
			data->line = ft_re_malloc(data->line, n);
			if (data->line == 0)
				return ;
		}
		data->line = ft_strcat(data->line, data->buf);
	}
}

static int	save_backup(t_list **fdlst, t_data *data, int fd)
{
	if (data->line[data->pos] == '\n')
		data->pos += 1;
	while (*fdlst != 0)
	{
		if ((*fdlst)->fd == fd)
		{
			if ((*fdlst)->content != 0)
				free((*fdlst)->content);
			(*fdlst)->content = ft_strdup(&(data->line[data->pos]));
			if ((*fdlst)->content == 0)
				return (0);
			data->line[data->pos] = '\0';
			return (1);
		}
		*fdlst = (*fdlst)->next;
	}
	return (1);
}

static void	free_fdnode(t_list **fdlst, int fd)
{
	t_list	*tmp;
	t_list	*previous;

	previous = 0;
	while ((*fdlst) != 0)
	{
		if ((*fdlst)->fd == fd)
		{
			tmp = (*fdlst)->next;
			if ((*fdlst)->content != 0)
				free((*fdlst)->content);
			free((*fdlst));
			*fdlst = tmp;
			if (previous != 0)
				previous->next = *fdlst;
			return ;
		}
		previous = *fdlst;
		*fdlst = (*fdlst)->next;
	}
}

char	*get_next_line(int fd)
{
	static t_list	*fdlst;
	t_data			data;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (chk_backup(&fdlst, &data, fd) == 0)
	{
		free_fdnode(&fdlst, fd);
		free(data.line);
		return (NULL);
	}
	read_file(&data, fd);
	free(data.buf);
	if (data.read_size <= 0 || data.line[0] == '\0')
	{
		free_fdnode(&fdlst, fd);
		if (data.read_size == 0 && data.line[0] != '\0')
			return (data.line);
		free(data.line);
		return (NULL);
	}
	if (save_backup(&fdlst, &data, fd) == 0 && data.line[data.pos] == '\0')
		free_fdnode(&fdlst, fd);
	return (data.line);
}