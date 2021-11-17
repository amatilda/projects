/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_log.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wstygg <wstygg@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 18:22:05 by wstygg            #+#    #+#             */
/*   Updated: 2021/01/09 13:44:14 by wstygg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf.h"
#include "conf_reader.h"

#include <time.h>

void			log_reopen(const char *path)
{
	t_log					*log;
	struct stat				st;

	log = get_log();
	if (log->fd)
		close(log->fd);
	log->fd = open(path, O_CREAT | O_RDWR | O_APPEND,
	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (log->fd)
	{
		if (fstat(log->fd, &st) == -1)
		{
			close(log->fd);
			return ;
		}
		log->size = st.st_size;
		free(log->path);
		log->path = strdup(path);
	}
}

static void		log_recreate(void)
{
	close(get_log()->fd);
	remove(get_log()->path);
	log_reopen(get_log()->path);
}

static void		task_print(const char *format, va_list list)
{
	char		buffer[0x400];//Не паримся текст не большой - стека ешь не хочу
	time_t		t;
	struct tm	tm;
	ssize_t		count1;
	ssize_t		count2;

	t = time(NULL);
	tm = *localtime(&t);
	if ((count1 = sprintf(&buffer[0], "[%02d-%02d-%d\t%02d:%02d:%02d]\n",
	tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
	tm.tm_hour, tm.tm_min, tm.tm_sec)) == -1)
		return ;
	if ((count2 = vsprintf(&buffer[count1], format, list)) == -1)
		return ;
	count1 = count1 + count2;
	buffer[count1] = '\n';
	buffer[count1 + 1] = '\n';
	count1 = count1 + 2;
	get_log()->size += count1;
	write(get_log()->fd, &buffer[0], count1);
}

void			task_log(const char *format, ...)
{
	va_list		list;

	if (get_log()->size >= MAX_LOG_SIZE)
		log_recreate();
	if (get_log()->fd)
	{
		va_start(list, format);
		task_print(format, list);
		va_end(list);
	}
}
