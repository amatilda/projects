/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gmtime_r.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_gmtime_r/inc/ft_gmtime_r.h"

static void		fn_time(register t_gmtime_r *gmt)
{
	gmt->result->tm_hour = 0;// 0 - 23
	while (gmt->unix_time >= 3600)//3600 - один час в секундах
	{
		gmt->unix_time -= 3600;
		gmt->result->tm_hour++;
	}
	gmt->result->tm_min = 0;// 0 - 59
	while (gmt->unix_time >= 60 && gmt->result->tm_min <= 59)//60 - одна минута в секундах; тем самым учитываем что сикунды от 0 - 60 - тоесть высикосную секунду
	{
		gmt->unix_time -= 60;
		gmt->result->tm_min++;
	}
	gmt->result->tm_sec = gmt->unix_time;
}

static void		fn_day(t_gmtime_r *gmt)
{
	gmt->result->tm_mday = 1;// 1 - 31
	while (gmt->unix_time >= 86400)
	{
		gmt->unix_time -= 86400;
		gmt->result->tm_mday++;
		gmt->result->tm_yday++;
	}
	gmt->common += 1 + 3 + gmt->result->tm_yday;//1 января 1970 года - четверг - 4;+ 3 догоняем до  7;+ 1 - число дней от 0 до 365 делаем от 1 до 366
	gmt->result->tm_wday = gmt->common % 7;//день недели (Воскресенье = 0)
}

static void		fn_month(register t_gmtime_r *gmt)
{
	register int_fast32_t month[12];

	month[0] = 31 * 86400;
	month[1] = gmt->bisextos > 31536000 ? 29 * 86400 : 28 * 86400;//проверяем высакосный год или нет
	month[2] = 31 * 86400;
	month[3] = 30 * 86400;
	month[4] = 31 * 86400;
	month[5] = 30 * 86400;
	month[6] = 31 * 86400;
	month[7] = 31 * 86400;
	month[8] = 30 * 86400;
	month[9] = 31 * 86400;
	month[10] = 30 * 86400;
	month[11] = 31 * 86400;
	gmt->result->tm_mon = 0;// 0 - 11
	gmt->result->tm_yday = 0;// 0 - 365
	while (gmt->unix_time >= month[gmt->result->tm_mon])
	{
		gmt->unix_time -= month[gmt->result->tm_mon];
		gmt->result->tm_yday += month[gmt->result->tm_mon];
		if (gmt->result->tm_mon == 11)
			break ;
		gmt->result->tm_mon++;
	}
	gmt->result->tm_yday /= 86400;//86400 - одни сутки в секундах
}

static void		fn_year(register t_gmtime_r *gmt)
{
	gmt->big = 1970;
	gmt->common = 0;
	gmt->bisextos = 31536000;//365 - дней - 1970 год - не высикосный
	while (gmt->unix_time >= gmt->bisextos)
	{
		gmt->unix_time -= gmt->bisextos;
		gmt->common = gmt->bisextos > 31536000 ? gmt->common + 366 : gmt->common + 365;
		gmt->big++;
		if (((gmt->big >> 2) << 2) != gmt->big)
			gmt->bisextos = 31536000;
		else if (gmt->big % 400 == 0)
			gmt->bisextos = 31536000 + 86400;//366 дней - высикосный год
		else if (gmt->big % 100 == 0)
			gmt->bisextos = 31536000;
		else
			gmt->bisextos = 31536000 + 86400;
	}
	gmt->result->tm_year = gmt->big - 1900;//в такой форме записываеться
}

struct tm		*ft_gmtime_r(const time_t *clock, struct tm *result)
{
	t_gmtime_r			gmt;

	gmt.unix_time = *clock;
	gmt.result = result;
	fn_year(&gmt);
	fn_month(&gmt);
	fn_day(&gmt);
	fn_time(&gmt);
	return (result);
}
