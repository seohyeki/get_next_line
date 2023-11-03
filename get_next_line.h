/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seohyeki <seohyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:22:18 by seohyeki          #+#    #+#             */
/*   Updated: 2023/11/04 06:40:20 by seohyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

typedef struct s_list
{
	int				fd;
	char			content[BUFFER_SIZE + 1];
	struct s_list	*next;
}	t_list;

typedef struct s_data
{
	int		pos;
	int		len;
	ssize_t	read_size;
	char	*buf;
	char	*line;
}	t_data;

int		ft_find_nl(const char *s, t_data *data);
char	*ft_strdup(const char *s1);
void	ft_strcat(char *dest, const char *src);
char	*ft_re_malloc(char *origin, int size);
int		add_newnode(t_list **previous, t_list **fdlst, int fd, t_data *data);
char	*get_next_line(int fd);

#endif