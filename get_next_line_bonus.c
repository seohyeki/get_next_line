/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seohyeki <seohyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 20:35:56 by seohyeki          #+#    #+#             */
/*   Updated: 2023/11/04 06:34:58 by seohyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	free_fdnode(t_list **fdlst, int fd)
{
	t_list	*curr;
	t_list	*previous;

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

	curr = *fdlst;
	while (curr != 0)
	{
		if (curr->fd == fd)
		{
			data->line = ft_strdup(curr->content);
			if (data->line == 0)
			{
				free_fdnode(fdlst, fd);
				return (0);
			}
			return (1);
		}
		previous = curr;
		curr = curr->next;
	}
	if (add_newnode(&previous, fdlst, fd, data) == 0)
	{
		free_fdnode(fdlst, fd);
		return (0);
	}
	return (1);
}

static void	read_file(t_data *data, int fd)
{
	int	n;

	n = 1;
	while (ft_find_nl(data->line, data) == 0)
	{
		data->read_size = read(fd, data->buf, BUFFER_SIZE);
		if (data->read_size <= 0)
			return ;
		data->buf[data->read_size] = '\0';
		if (data->len + data->read_size >= BUFFER_SIZE * n)
		{
			n *= 2;
			data->line = ft_re_malloc(data->line, BUFFER_SIZE * n);
			if (data->line == 0)
				return ;
		}
		ft_strcat(data->line, data->buf);
	}
	if (data->line[data->pos] == '\n')
		data->pos++;
}

static int	save_backup(t_list **fdlst, t_data *data, int fd)
{
	t_list	*cur;

	cur = *fdlst;
	while (cur != 0)
	{
		if (cur->fd == fd)
		{
			cur->content[0] = '\0';
			ft_strcat(cur->content, &(data->line[data->pos]));
			data->line = ft_re_malloc(data->line, data->pos);
			if (data->line == 0 || cur->content[0] == '\0')
				return (0);
			else
				return (1);
		}
		cur = cur->next;
	}
	data->line = ft_re_malloc(data->line, data->pos);
	if (data->line == 0)
		return (0);
	return (1);
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
		free_fdnode(&fdlst, fd);
		return (NULL);
	}
	read_file(&data, fd);
	free(data.buf);
	if (save_backup(&fdlst, &data, fd) == 0)
		free_fdnode(&fdlst, fd);
	if ((data.read_size == -1 || data.line[0] == '\0') \
		&& ft_find_nl(data.line, &data) == 0)
	{
		free(data.line);
		return (NULL);
	}
	return (data.line);
}
