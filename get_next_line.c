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

#include "get_next_line.h"

static char	*line_split(char *str, char **remain)
{
	char	*tmp;
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
	tmp = str;
	result = ft_substr(str, 0, idx + 1);
	*remain = ft_substr(str, idx + 1, ft_strlen(str));
	free(tmp);
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
	static char	*backup;
	char		*tmp;
	int			read_size;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (backup && is_nl(backup) >= 0)
		return (line_split(backup, &backup));
	while (1)
	{
		tmp = read_and_append(fd, backup, &read_size);
		if (!tmp)
			return (free(backup), backup = NULL, NULL);
		backup = tmp;
		if (is_nl(backup) >= 0)
			return (line_split(backup, &backup));
		if (read_size == 0)
			break ;
	}
	if (!backup || !*backup)
		return (free(backup), backup = NULL, NULL);
	tmp = backup;
	backup = NULL;
	return (tmp);
}
