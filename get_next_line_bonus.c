/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seohyeki <seohyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 20:35:56 by seohyeki          #+#    #+#             */
/*   Updated: 2023/11/01 23:52:52 by seohyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

//backup fd노드가 있는지 찾고 있으면 가져오고 없으면 새로하나 만들어두기
static int	chk_backup(t_list **fdlst, t_data *data, int fd)
{
	t_list	*previous;
	t_list  *new;

	previous = 0;
	//백업된 fd가 있는지 찾기
	while (*fdlst != 0)
	{
		if ((*fdlst)->fd == fd)//있으면 line에 strdup
		{
			data->line = ft_strdup((*fdlst)->content);
			if (data->line == 0)
				return (0);
			return (1);
		}
		previous = *fdlst;
		*fdlst = (*fdlst)->next;
	}
  	new = add_newnode(fd);
	if (new == 0)
		return (0);
	if (previous == 0) //아예 노드가 없을때 
		*fdlst = new;
	else //다른 fd노드 있음
		previous->next = new;
	//line 버퍼사이즈 만큼 할당하고 맨앞에 \0
	data->line = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (data->line == 0)
		return (0);
	data->line[0] = '\0'; //line에 cat으로 붙이기 때문에 넣어줘야돼
	return (1);
}

//개행있을때까지 읽기
static void	read_file(t_data *data, int fd)
{
	int	n; //line크기를 2배로 늘리려고

	n = 1;
	//버퍼 말록
	data->buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (data->buf == NULL)
		return ;
	data->buf[BUFFER_SIZE] = '\0';
	while (ft_find_nl(data->line, data) == 0) //개행 있으면 1, 없으면 0
	{
		data->read_size = read(fd, data->buf, BUFFER_SIZE);
		if (data->read_size <= 0) //read에러 or EOF
			return ;
		data->buf[data->read_size] = '\0'; //남은게 버퍼사이즈보다 작아서 적게 있었을때를 위해서 필요한 코드
		if (data->len + data->read_size >= BUFFER_SIZE * n) //꽉찼으면 기존*2로 line 재할당
		{
			n *= 2;
			data->line = ft_re_malloc(data->line, BUFFER_SIZE * n);
			if (data->line == 0) //재할당 실패
				return ;
		}
		data->line = ft_strcat(data->line, data->buf); //자리있으니깐 붙여넣기
	}
}

//개행이후 문자열이 남아있다면 저장하고 line 딱맞게 재할당
static int	save_backup(t_list **fdlst, t_data *data, int fd)
{
	if (data->line[data->pos] == '\n')
		data->pos += 1;
	while (*fdlst != 0)
	{
		if ((*fdlst)->fd == fd)
		{
			if ((*fdlst)->content != 0)
			{
				free((*fdlst)->content);
				(*fdlst)->content = 0;
			}
			if (data->line[data->pos] != '\0')
			{
				(*fdlst)->content = ft_strdup(&(data->line[data->pos]));
				if ((*fdlst)->content == 0)
					return (0);
			}
			else
			{
				data->line[data->pos] = '\0';
				return (0);
			}
			data->line[data->pos] = '\0';
			//data->line = ft_re_malloc(data->line, data->pos);//line 재할당 널가드는...
     		//if (data->line == 0)
				//return (0);
			return (1);
		}
		*fdlst = (*fdlst)->next;
	}
	//data->line = ft_re_malloc(data->line, data->pos);//line 재할당 근데 널가드..
	//if (data->line == 0)
		//return(0);
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
    	//data.line = ft_re_malloc(data.line, data.pos);
		if (data.read_size == 0 && data.line[0] != '\0') //끝까지 읽었고 리턴할 내용도 있을때
       		return (data.line);//리턴하기전에 말록 다시해야하는데....
		free(data.line);
    	return (NULL);
	}
	if (save_backup(&fdlst, &data, fd) == 0)
		free_fdnode(&fdlst, fd);
	return (data.line);
}