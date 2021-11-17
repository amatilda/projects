/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_crc32_mirror_one.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 16:45:36 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/06 21:16:19 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>

uint32_t	ft_crc32_mirror_one(register uint32_t *lp, register void *src,//lp - указаьель на таблицу работает с глубиной 1 - не будем делать парлеризацию//src - указатель на данные для подсчета
size_t count, register uint32_t crc32)//count - сколько данных в src//crc32 - начальное значение обычыно 0xFFFFFFFF
{
	register void			*src_end;
	register uint8_t		tempos;

	src_end = src + count;
	while (src < src_end)
	{
		tempos = ((char *)src++)[0];
		tempos = tempos ^ (uint8_t)crc32;
		crc32 = crc32 >> 8;
		crc32 = crc32 ^ lp[tempos];
	}
	crc32 = ~crc32;//Обратим аналог ассемблерной not так обычно вроде делают
	return (crc32);
}
