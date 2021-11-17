/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_cm_jobs_killgrp.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_cm.h"

void			ft_42sh_cm_jobs_killgrp(register t_jobs_42sh *jobs,
register int signo)
{
	register pid_t				pid;
	register size_t				count;

	count = jobs->count;
	while (count-- > 1)
		jobs = jobs->prev;//Отмотаем на начало пака jobs
	while (0xFF)
	{
		if ((pid = jobs->pid_view) != 0)//Для отображения pid в утилите jobs, если ноль значит функция которая без пайпа и не запускалась с помощью fork//должно быть зануленно//Первый в паке это так же и pid группы кроме скрипта но там мы и не юзаем fg и bg
		{
			kill(~pid + 1, signo);//Отправим все группе процессов приостановленной
			return ;
		}
		if ((jobs = jobs->next) == 0 || jobs->count == 1)
			return ;
	}
}
