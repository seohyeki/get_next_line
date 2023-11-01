/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seohyeki <seohyeki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:22:18 by seohyeki          #+#    #+#             */
/*   Updated: 2023/11/01 00:57:03 by seohyeki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_list
{
	int				fd;
	char			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_data
{
	char	*buf;
	char	*line;
	ssize_t	read_size;
	int		pos;
	int		len;
}	t_data;

int		ft_find_nl(const char *s, t_data *data);
char	*ft_strdup(const char *s1);
char	*ft_strcat(char *dest, const char *src);
char	*ft_re_malloc(char *origin, int n);
t_list	*add_newnode(int fd);
char	*get_next_line(int fd);

#endif