/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

void			ft_42sh_read(register t_main_42sh *array)
{
	register size_t			count;

	ft_42sh_dsp_start(array);
	ft_write_buffer(&array->out);
	while (0xFF)//входим в бесконечный цикл
	{
		array->litter[0] = 0;//Обнуляем что бы убрать предыдущий муссар
		count = read(FD_TERMINAL_42SH, &array->litter, sizeof(array->litter));
		if ((ssize_t)count == -1)
		{
			if ((array->b_mode & MODE_DEBUG_42SH) != 0)
				continue ;
			ft_42sh_exit(E_READ_CODE_42SH, __FILE__, __func__, __LINE__);
		}
		if (array->b_read != 0)//Проверим надо ли врстановить режим считывания на режим по умолчанию//Что бы не выходила с read каждую милисекунду
		{
			array->b_read = 0;
			array->tty_change.c_cc[VMIN] = 1;//Покуда не считает хоть один файл read не заканчивает свою работу
			array->tty_change.c_cc[VTIME] = 0;//Отменяем ограничения по времени на ожидание
			if (ft_42sh_stub_ioctl(array, TIOCSETA, &array->tty_change) == -1)//Изменили состояние терминала
				ft_42sh_exit(E_IOTL_CODE_42SH, __FILE__, __func__, __LINE__);
		}
		ft_42sh_read_finish(array, count);
	}
}
