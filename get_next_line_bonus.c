/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seohyeki <seohyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 20:35:56 by seohyeki          #+#    #+#             */
/*   Updated: 2023/11/05 20:05:53 by seohyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	free_fdnode(t_list **fdlst, t_data *data, int fd)
{
	t_list	*curr;
	t_list	*previous;

	free(data->buf);
	data->buf = 0;
	if (*fdlst == 0)
		return ;
	curr = *fdlst;
	if (curr->fd == fd)
	{
		*fdlst = curr->next;
		free(curr);
		return ;
	}
	while (curr != 0)
	{
		if (curr->fd == fd)
		{
			previous->next = curr->next;
			free(curr);
			return ;
		}
		previous = curr;
		curr = curr->next;
	}
}

static int	chk_backup(t_list **fdlst, t_data *data, int fd)
{
	t_list	*previous;
	t_list	*curr;

	data->line = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (data->line == 0)
		return (0);
	data->line[0] = '\0';
	curr = *fdlst;
	while (curr != 0)
	{
		if (curr->fd == fd)
		{
			ft_strcat(data->line, curr->content);
			return (1);
		}
		previous = curr;
		curr = curr->next;
	}
	if (add_newnode(&previous, fdlst, fd) == 0)
	{
		free(data->line);
		return (0);
	}
	return (1);
}

static int	read_file(t_data *data, int fd)
{
	int	n;

	n = 1;
	while (ft_find_nl(data->line, data) == 0)
	{
		data->read_size = read(fd, data->buf, BUFFER_SIZE);
		if (data->read_size <= 0)
			return (0);
		data->buf[data->read_size] = '\0';
		if (data->len + data->read_size >= BUFFER_SIZE * n)
		{
			n *= 2;
			data->line = ft_re_malloc(data->line, BUFFER_SIZE * n);
			if (data->line == 0)
				return (0);
		}
		ft_strcat(data->line, data->buf);
	}
	if (data->line[data->pos] == '\n')
		data->pos++;
	return (1);
}

static void	save_backup(t_list **fdlst, t_data *data, int fd)
{
	t_list	*curr;

	curr = *fdlst;
	while (curr != 0)
	{
		if (curr->fd == fd)
		{
			curr->content[0] = '\0';
			ft_strcat(curr->content, &(data->line[data->pos]));
			data->line = ft_re_malloc(data->line, data->pos);
			if (data->line == NULL || curr->content[0] == '\0')
			{
				free_fdnode(fdlst, data, fd);
				return ;
			}
			free(data->buf);
			return ;
		}
		curr = curr->next;
	}
	free(data->buf);
	return ;
}

char	*get_next_line(int fd)
{
	static t_list	*fdlst;
	t_data			data;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	data.buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (data.buf == NULL || chk_backup(&fdlst, &data, fd) == 0)
	{
		free_fdnode(&fdlst, &data, fd);
		return (NULL);
	}
	if (read_file(&data, fd) == 0)
	{
		free_fdnode(&fdlst, &data, fd);
		if (data.read_size == 0 && data.line[0] != '\0')
		{
			data.line = ft_re_malloc(data.line, data.pos);
			return (data.line);
		}
		free(data.line);
		return (NULL);
	}
	save_backup(&fdlst, &data, fd);
	return (data.line);
}
