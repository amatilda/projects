/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_display_x.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_printf.h"

static int	fn_finish(register t_my *my, register char *buffer, register size_t count)
{
	my->opt.min = my->opt.min > my->tmp ? my->opt.min - my->tmp : 0;
	my->tmp += my->opt.min;
	if (my->tmp >= INT_MAX)//printf в MAC ограничивает на такое число
		return (0);
	else if (my->out + my->count >= INT_MAX - my->tmp)//отнимаем так как мы не импользуем int
		return (0);
	if (my->opt.letter != 0 && my->opt.min != 0 && my->opt.b_null != 0 && my->opt.b_max == 0)
	{
		fn_str_copy(my, &my->opt.letter, 1);
		my->opt.letter = 0;
	}
	if (my->opt.b_left == 0 && my->opt.b_max != 0 && my->opt.min != 0)
		fn_sp(my, ' ');
	if (my->opt.b_left == 0 && my->opt.min != 0 && (my->format == 'x' || my->format == 'X') && my->opt.b_null == 0)
		fn_sp(my, my->sp);//заполняем пробелом или нулем в зависимости от наличия префикса и b_max
	if (my->opt.b_prefix != 0 && my->opt.base == 16 && my->format != 'p' && my->format != 'P')
	{
		fn_str_copy(my, (my->format < 'a') ? "0X" : "0x", 2);
		my->opt.b_prefix = 0;
	}
	if (my->opt.b_left == 0 && my->opt.min != 0)
		fn_sp(my, my->sp);//заполняем пробелом или нулем в зависимости от наличия префикса и b_max
	if (my->opt.b_prefix != 0 && my->opt.base == 16)
		fn_str_copy(my, (my->format < 'a') ? "0X" : "0x", 2);
	if (my->opt.letter != 0)
		fn_str_copy(my, &my->opt.letter, 1);
	if (my->opt.max != 0)
	{
		my->tmp =  my->opt.min;//сохраняем что бы не затереть
		my->opt.min = my->opt.max;
		fn_sp(my, (my->opt.b_max_over != 0) ? 0x20 : 0x30);
		my->opt.min = my->tmp;//востанавливаем
	}
	fn_str_copy(my, buffer,count);
	if (my->opt.min != 0)//и в конце если надо заполняем пробелами
		fn_sp(my, ' ');
	return (1);
}

static void	fn_stub1(register t_my *my, register char **buffer, register size_t *count)
{
	if (my->opt.minus != 0 && my->opt.b_space != 0 && my->opt.letter == 0)//если с системой работающей с отрицательными числами и перед числом нет + или - и есть b_space ставим пробел
		my->opt.letter = ' ';
	my->tmp = my->opt.letter != 0 ? 1 : 0;
	if (my->opt.b_prefix != 0)
	{
		if (my->opt.base == 16)
			my->tmp += 2;
		else if (my->opt.base == 8 && my->arg_itoa != 0)//если 8 система исчисления и префик то ставим перед 0 и если есть групировка и длинна числа = 3 то ставим пробел
		{//символ групировке printf должен получаться с помощью localeconv но не уверен что можно ее использовать - зашиваем как пробел, тоже и в itoa
			*buffer = *buffer - 1;
			*count = *count + 1;
			if (*count == 4 && my->opt.b_grup != 0)
			{
				*buffer[0] = ' ';
				*buffer = *buffer - 1;
				*count = *count + 1;
			}
			*buffer[0] = 0x30;
			my->opt.max -= my->opt.max > 0 ? 1 : 0;
		}
	}
}

static void	fn_stub0_0(register t_my *my)
{
	if (my->opt.minus != 0)//проверяем имеем дело с системой работающей с отрицательными числами(d D i) или нет
	{
		if (my->opt.b_sub[3] != 0 || my->opt.b_sub[4] != 0)
			my->arg_itoa = (ssize_t)my->arg_itoa;
		else if (my->opt.b_sub[sizeof(STR_SUB)] != 0)
			my->arg_itoa = (long long)my->arg_itoa;
		else if (my->opt.b_sub[1] != 0)
			my->arg_itoa = (long)my->arg_itoa;
		else if (my->opt.b_sub[0] != 0)
			my->arg_itoa = (short)my->arg_itoa;
		else if (my->opt.b_sub[sizeof(STR_SUB) + 1] != 0)
			my->arg_itoa = (signed char)my->arg_itoa;
		else
			my->arg_itoa = (signed int)my->arg_itoa;
		return ;
	}
	if (my->opt.b_sub[3] != 0 || my->opt.b_sub[4] != 0)
		return ;
	else if (my->opt.b_sub[sizeof(STR_SUB)] != 0)
		my->arg_itoa = (unsigned long long)my->arg_itoa;
	else if (my->opt.b_sub[1] != 0)
		my->arg_itoa = (unsigned long)my->arg_itoa;
	else if (my->opt.b_sub[0] != 0)
		my->arg_itoa = (unsigned short)my->arg_itoa;
	else if (my->opt.b_sub[sizeof(STR_SUB) + 1] != 0)
		my->arg_itoa = (unsigned char)my->arg_itoa;
	else
		my->arg_itoa = (unsigned int)my->arg_itoa;
}

static int	fn_stub0(register t_my *my, register char *buffer)
{
	register uint_fast8_t 	op;

	my->opt.base = (my->opt.base == 0) ? 16 : my->opt.base;//выбираем базу для преобразования
	op = (my->format < 'a') ? ITOA_UPPER : ITOA_LOWER;
	op = (my->opt.base != 16 && my->opt.b_grup != 0) ? op | ITOA_GROUPED : op;
	op = (my->opt.minus != 0 && my->opt.b_plus != 0) ? op | ITOA_PLUS : op;
	op = op | my->opt.minus;
	if (my->opt.b_sub[2] != 0)
		my->arg_itoa = va_arg(my->ap_line, uintmax_t);
	else
	{
		my->arg_itoa =  va_arg(my->ap_line, size_t);
		fn_stub0_0(my);
	}
	if (my->arg_itoa == 0 && my->format != 'p' && my->format != 'P' && my->opt.base != 8)
		my->opt.b_prefix = 0;
	return (ft_itoa(buffer, my->arg_itoa, my->opt.base, op));
}

int		fn_display_x(register t_my *my)
{
	char			str[sizeof(uintmax_t)*10 + 2 + 2];//максимум займут цифры, *8 на двоиичную сис исчисл, *2 на пробелы между тысячаси, +2 на 0x,+2 на пробел и плюс - стек не паримся
	char			*buffer;
	size_t			count;

	buffer = str + 2;//что бы не было лишник операций поэтому добавляем перед числом где вазможно и каректируем
	count = fn_stub0(my, buffer);
	if (buffer[0] < 0x30)
	{
		my->opt.letter = buffer[0];
		buffer++;
		count--;
	}
	if (my->opt.b_max != 0)
	{
		if (my->opt.max == 0 && my->arg_itoa == 0)
		{
			if (my->opt.base == 10)
				count = 0;
			else if (my->opt.base != 8)
			{
				my->opt.letter = 0;
				count = 0;
			}
			else if (my->opt.b_prefix == 0 && my->opt.base == 8)
			{
				my->opt.letter = 0;
				count = 0;
			}
		}
		if (my->opt.max >= INT_MAX)//printf в MAC ограничивает на такое число; так как мы не импользуем int
			return (0);
		my->opt.max = my->opt.max > count ? my->opt.max - count : 0;
	}
	fn_stub1(my, &buffer, &count);
	my->tmp += count + my->opt.max;
	return (fn_finish(my, buffer, count));
}