/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaehlee <jaehlee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:45:33 by jaehlee           #+#    #+#             */
/*   Updated: 2025/05/09 17:45:33 by jaehlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*line_split(char *str, char **remain)
{
	char	*result;
	int		idx;

	if (!str)
		return (NULL);
	idx = is_nl(str);
	if (idx < 0)
	{
		*remain = NULL;
		return (str);
	}
	result = ft_substr(str, 0, idx + 1);
	*remain = ft_substr(str, idx + 1, ft_strlen(str));
	free(str);
	return (result);
}

static char	*read_and_append(int fd, char *backup, int *read_size)
{
	char	*buf;
	char	*tmp;

	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	*read_size = read(fd, buf, BUFFER_SIZE);
	if (*read_size == -1)
	{
		free(buf);
		return (NULL);
	}
	if (*read_size == 0)
	{
		free(buf);
		return (backup);
	}
	buf[*read_size] = '\0';
	tmp = ft_strjoin(backup, buf);
	free(buf);
	return (tmp);
}

char	*get_next_line(int fd)
{
	static char	*backup[OPEN_MAX];
	char		*tmp;
	int			read_size;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (backup[fd] && is_nl(backup[fd]) >= 0)
		return (line_split(backup[fd], &backup[fd]));
	while (1)
	{
		tmp = read_and_append(fd, backup[fd], &read_size);
		if (!tmp)
			return (free(backup[fd]), backup[fd] = NULL, NULL);
		backup[fd] = tmp;
		if (is_nl(backup[fd]) >= 0)
			return (line_split(backup[fd], &backup[fd]));
		if (read_size == 0)
			break ;
	}
	if (!backup[fd] || !*backup[fd])
		return (free(backup[fd]), backup[fd] = NULL, NULL);
	tmp = backup[fd];
	backup[fd] = NULL;
	return (tmp);
}
