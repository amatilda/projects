/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_big.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_printf.h"

static void		fn_stub3(register t_my *my)
{
	my->count++;
	if (my->b_in_str == 0 && my->count == my->size_b)
	{
		my->out += my->count;
		my->count = 0;
		write(my->fildes, my->b, my->size_b);
	}
}

static size_t	fn_stub2(register t_my *my)
{
	if (my->b_in_str != 0)
		my->b[my->count < my->size_b ? my->count : my->size_b] = 0;
	else if (my->count > 0)
		write(my->fildes, my->b, my->count);
	va_end(my->ap);
	va_end(my->ap_line);
	va_end(my->ap_n);
	return (my->out + my->count);
}

static void		fn_stub1(register t_my *my)
{
	my->stc->str_format = STR_FORMAT;
	my->stc->str_sub = STR_SUB;
	my->stc->fn[0] = fn_display_unknown;//unknown
	my->stc->fn[1] = fn_display_s;//s
	my->stc->fn[2] = fn_display_s;//S
	my->stc->fn[3] = fn_display_p;//p
	my->stc->fn[4] = fn_display_p;//P
	my->stc->fn[5] = fn_display_d;//d
	my->stc->fn[6] = fn_display_d;//D
	my->stc->fn[7] = fn_display_d;//i
	my->stc->fn[8] = fn_display_o;//o
	my->stc->fn[9] = fn_display_o;//O
	my->stc->fn[10] = fn_display_u;//u
	my->stc->fn[11] = fn_display_u;//U
	my->stc->fn[12] = fn_display_x;//x
	my->stc->fn[13] = fn_display_x;//X
	my->stc->fn[14] = fn_display_c;//c
	my->stc->fn[15] = fn_display_c;//C
	my->stc->fn[16] = fn_display_n;//n
	my->stc->fn[17] = fn_display_b;//b
	my->stc->fn[18] = fn_display_b;//B
	my->stc->fn[19] = fn_display_k;//k
	my->stc->fn[20] = fn_display_f;//f
	my->stc->fn[21] = fn_display_f;//F
	my->stc->fn[22] = fn_display_e;//e
}

static void		fn_stub0(register t_my *my, register t_arg *arg)
{
	static t_static	stc;

	my->size_b = arg->size_b;
	my->fildes = arg->fildes;
	my->f = arg->f;
	my->b = arg->b == 0 ? my->buffer : arg->b;
	my->b_in_str = arg->b_in_str;
	my->end_count = 0;
	my->count = 0;
	my->out = 0;
	va_copy(my->ap, arg->ap);
	va_copy(my->ap_line, my->ap);
	va_copy(my->ap_n, my->ap);
	my->stc = &stc;
	if (my->stc->fn[26] != 0)//последнюю все же лучше чем ничего, вслучае с паралельными потоками нужна атомарность sig_atomic_t !!!
		return ;
	fn_stub1(my);
	my->stc->fn[23] = fn_display_e;//E
	my->stc->fn[24] = fn_display_a;//a
	my->stc->fn[25] = fn_display_a;//A
	my->stc->fn[26] = fn_display_r;//r
}

size_t			fn_big(register t_arg *arg)
{
	t_my	my;//выделяем большой буфер стек для этого - не айс, но в случаи многопоточности или стек или malloc

	fn_stub0(&my, arg);
	while (my.f[0] != 0)
	{
		if (my.f++[0] == '%')
		{
			if (fn_display(&my) == 0)
			{
				my.count = my.count > my.end_count ? my.end_count : my.count;
				if (my.b_in_str != 0)
					my.b[my.count < my.size_b ? my.count : my.size_b] = 0;
				else if (my.count > 0)
					write(my.fildes, my.b, my.count);
				return (-1);
			}
			my.end_count = my.count--;
		}
		else if (my.out + my.count >= INT_MAX - 1)//printf в MAC ограничивает на такое число - минус один на планируемое записать число
			return (-1);
		else if (my.count < my.size_b)
			my.b[my.count] = my.f[-1];
		fn_stub3(&my);
	}
	return (fn_stub2(&my));
}