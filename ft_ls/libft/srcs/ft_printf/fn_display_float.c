/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_display_float.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_printf.h"

static void fn_test_number(register t_my *my)
{
	my->flt.exponent &= 0x7FFF;
	my->flt.e_save = my->flt.exponent - 0x3FFF;
	if (my->flt.e_save == -16383 && my->flt.mantissa == 0)
		my->flt.buffer[my->opt.count++] = '0';
	else if (my->flt.e_save == 0x4000)
	{
		my->flt.letter_tmp = 'n' + my->flt.up;
		if (my->flt.mantissa == 0x8000000000000000)
		{
			my->flt.buffer[0] = 'i' + my->flt.up;
			my->flt.buffer[1] = my->flt.letter_tmp;
			my->flt.buffer[2] = 'f' + my->flt.up;
		}
		else
		{
			my->opt.letter = 0;
			my->flt.buffer[0] = my->flt.letter_tmp;
			my->flt.buffer[1] = 'a' + my->flt.up;
			my->flt.buffer[2] = my->flt.letter_tmp;
		}
		my->opt.max = 0;
		my->sp = ' ';
		my->opt.count = 3;
	}
}

static void fn_init_sing(register t_my *my, register long double value)
{
	while (value >= PRINTF_MLT_DEC)//для ускарения
	{
		value /= PRINTF_MLT_DEC;
		my->opt.sing += PRINTF_MLT_DEC_C;
	}
	while (value >= 1)//получаем такой тип: 0.xxxxx
	{
		value /= my->opt.base;
		my->opt.sing++;
	}
	if (my->opt.b_exp != 0)
	{
		while (value < 1)//приводим к 1.xxxx
		{
			value *= my->opt.base;
			my->opt.e--;
		}
		return ;
	}
	else if (my->opt.b_grup == 0 || my->opt.sing <= 3)
		return ;
	my->opt.grup = my->opt.sing / 3;//подсчитываем необходимое количество пробелов
	my->opt.grup -= (my->opt.sing == my->opt.grup * 3) ? 1 : 0;//если надо каректируем
}

static void fn_init(register t_my *my)
{
	if ((my->flt.exponent & 0x8000) != 0)
		my->opt.letter = '-';
	else if (my->opt.b_plus != 0)
		my->opt.letter = '+';
	else if (my->opt.b_space != 0)
		my->opt.letter = ' ';
	fn_test_number(my);
	if (my->opt.count != 0)
		return ;
	if (my->opt.b_ox != 0)//приводим к 1000b.xxxxxxx
	{
		my->flt.exponent = 16386;//число в мантисе всегда 1.xxxxxx поэтому делаем експоненту на +3b
		return ;
	}
	fn_init_sing(my, my->flt.value);
}

static int fn_finish(register t_my *my)
{
	my->tmp = (my->opt.sing != 0) ? my->opt.sing - 1 : 0;
	my->tmp += my->opt.max + (my->opt.count > 1 ? my->opt.count : 0) + 1;//- 1 учитываем что один одна цифра всегда должна быть так или иначе+
	my->tmp += (my->opt.letter != 0 ? 1 : 0) + my->opt.grup + my->opt.b_ox;
	if (my->tmp >= INT_MAX)//printf в MAC ограничивает на такое число; так как мы не импользуем int
		return (0);
	my->opt.min = my->opt.min > my->tmp ? my->opt.min - my->tmp: 0;
	my->flt.first = &my->b[my->count];
	if (my->opt.letter != 0 && my->opt.count <= 1)
		fn_str_copy(my, &my->opt.letter, 1);
	if (my->opt.b_left == 0 && my->opt.min != 0)
		fn_sp(my, my->sp);//заполняем пробелом или нулем
	if (my->opt.letter != 0 && my->opt.count > 1)
		fn_str_copy(my, &my->opt.letter, 1);
	if (my->opt.b_ox != 0 && my->opt.count <= 1)
		fn_str_copy(my, my->flt.up == 0 ? "0x" : "0X", 2);
	if (my->opt.count > 1)
		fn_str_copy(my, my->flt.buffer, 3);
	else
		fn_write_float(my);
	if (my->opt.b_exp != 0)
		fn_str_copy(my, my->flt.str, my->opt.sing - 1);
	if (my->opt.min != 0)
		fn_sp(my, ' ');//заполняем пробелом
	return (1);
}

int			fn_display_float(register t_my *my)
{
	my->opt.max = (my->opt.b_max == 0) ? 6 : my->opt.max;//6 - знаков после запитой по умолчанию
	my->opt.max +=  my->opt.max != 0 ? 1 : 0;//+1 для запитой учитываем
	my->flt.alphabet = (my->format < 'a') ? "0123456789ABCDEF" : "0123456789abcdef";
	my->flt.up = 'a' - my->flt.alphabet[10];
	my->flt.value = my->opt.b_sub[5] != 0 ? va_arg(my->ap_line, long double) : va_arg(my->ap_line, double);
	fn_init(my);
	if (my->opt.max >= INT_MAX)//printf в MAC ограничивает на такое число; так как мы не импользуем int
		return (0);
	if (my->opt.b_exp != 0 && my->opt.count <= 1)//добавляем на конце числа e+50 или p+50
	{
		my->flt.str[0] = my->format;
		if (my->opt.b_ox == 0)
		{
			my->opt.e = (my->opt.sing != 0) ? my->opt.sing - 1 : my->opt.e;
			my->opt.sing = ft_itoa(&my->flt.str[1], my->opt.e, 10, PRINTF_E) + 2;
		}
		else
		{
			my->opt.e = my->flt.e_save - 3;
			my->opt.sing = ft_itoa(&my->flt.str[1], my->opt.e, 10, PRINTF_P) + 2;//p+50 в види смещения двуричной системы исчи
		}
	}
	return (fn_finish(my));
}
