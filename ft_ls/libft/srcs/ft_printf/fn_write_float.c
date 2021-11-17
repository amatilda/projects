/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_write_float.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_printf.h"

static void		fn_pre(register t_my *my, register int16_t e_save, register uint64_t mantissa)
{
	if (e_save < 0)
	{
		e_save = ~e_save + 1;
		my->flt.float_fract_e = e_save - 1;//учитываем что в long double запитая стаит так: 1,xxxxxx
		while ((mantissa & 1) == 0)//смещаем в право что бы получить дробную часть
			mantissa = mantissa >> 1;
		my->flt.float_fract = mantissa;
	}
	else if (e_save >= 63)
	{
		my->flt.float_sing = mantissa;
		my->flt.float_sing_e = e_save - 63;
	}
	else
	{
		my->flt.float_sing = mantissa >> (63 - e_save);
		mantissa = mantissa << (e_save + 1);//учитываем что в long double запитая стаит так: 1,xxxxxx
		mantissa = mantissa >> (e_save + 1);//учитываем что в long double запитая стаит так: 1,xxxxxx
		while ((mantissa & 1) == 0)
			mantissa = mantissa >> 1;
		my->flt.float_fract = mantissa;
	}
}

void			fn_write_float(register t_my *my)
{
	my->flt.float_sing = 0;//целая часть
	my->flt.float_sing_e = 0;//сколько нулей добавить к целой части справа
	my->flt.float_fract = 0;//дробная часть
	my->flt.float_fract_e = 0;//сколько нулей добавить к дробной части слева
	//my->opt.max - точность и +1 на точку  - если равно нулю то дробная часть не выводиться
	fn_pre(my, my->flt.e_save, my->flt.mantissa);
}