/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_printf.h"

static int	fn_finish(register t_my *my)
{
	if (my->f[0] == '.')
	{
		my->opt.b_max = 1;
		my->opt.max = ft_atoi(my->f + 1);
		while (my->f[1] >= 0x30 && my->f[1] <= 0x39)
			my->f++;
	}
	else if (my->f[0] == 0x30)
		my->opt.b_null = 1;
	else if (my->f[0] == ' ')
		my->opt.b_space = 1;
	else if (my->f[0] == '+')
		my->opt.b_plus = 1;
	else if (my->f[0] == '-')
		my->opt.b_left = 1;
	else if (my->f[0] == '\'')
		my->opt.b_grup = 1;
	else if ((my->f[0] == 'l' || my->f[0] == 'h') && my->f[0] == my->f[1])
		my->opt.b_sub[sizeof(STR_SUB) + ((my->f++[0] == 'l') ? 0 : 1)] = 1;
	else if ((my->str_tmp_sub = ft_strchr(my->stc->str_sub, my->f[0])) != 0)
		my->opt.b_sub[my->str_tmp_sub - my->stc->str_sub] = 1;
	else
		return (0);
	return (1);
}

static void	fn_stub1(register t_my *my)
{
	my->b_point = my->f[-1] == '.' ? 1 : 0;//Если точка со звездой и доларом - то вступает очередное тупое правило - учитываем его
	my->b_star = my->f[0] == '*' ? 1 : 0;//установим обрабатываем в данный момент звезду или нет
	my->f = my->f[0] == '*' ? my->f + 1 : my->f;//если звезда сместим указатель
	my->null = my->f[0];//сохраним символ, что бы знать может это символ - ноль
	my->tmp = 0;// обнулим что бы использовать еще и в условии
	if (my->f[0] >= 0x30 && my->f[0] <= 0x39)//если число а не какойто знак то получим его
	{
		my->tmp = ft_atoi(my->f);
		while (my->f[1] >= 0x30 && my->f[1] <= 0x39)
			my->f++;
		my->f = my->f[1] == '$' ? my->f + 1 : my->f;//если после числа есть доллар значит увеличим указатель
	}
}

static int	fn_stub0(register t_my *my)
{
	fn_stub1(my);
	if (my->f[0] != '$')//проверим указатель указывает на доллар или нет, если да обрабатываем как доллар все
		return (1);
	if (my->b_point != 0)
	{
		va_copy(my->ap_n, my->ap_line);
		my->opt.max = va_arg(my->ap_n, size_t);
		my->opt.b_max = 1;
	}
	else if (my->tmp == 0)//Долар после звезд без цифер принимаем за 0
		my->opt.min = 0;
	else if (my->b_star != 0)//Раз перед ним есть звезда значит конец доларового выражения
	{
		va_copy(my->ap_n, my->ap);//что бы не прерывать не прирывную линию используем другую копию
		while (my->tmp-- > 0)
			my->opt.min = va_arg(my->ap_n, size_t);
	}
	else
	{
		va_copy(my->ap_line, my->ap);//Раз нет звезды перед значит начинаем новую линию
		while (my->tmp-- > 1)//причем подгоняем аргументы на один меньше, так там все тупо сделанно
			va_arg(my->ap_line, size_t);
	}
	return (0);
}

static int	fn_flag(register t_my *my)
{
	if ((my->f[0] >= 0x31 && my->f[0] <= 0x39) || my->f[0] == '*')
	{
		if (fn_stub0(my) == 0)
			my->f = my->f;
		else if (my->b_point != 0)
		{
			my->f--;//востановим указатель ведь в цикле потом увеличиться...
			my->opt.max = va_arg(my->ap_line, size_t);
			my->opt.b_max = 1;
		}
		else if (my->b_star != 0)//если стоит что звезда значит обрабатываем просто как звезду после которой нет долаллара в притык
		{
			my->f--;//востановим указатель ведь в цикле потом увеличиться...
			my->opt.min = va_arg(my->ap_line, size_t);//получим значение звезды
			my->opt.b_null = my->null == 0x30 ? 1 : my->opt.b_null;//раз просто звезда, то проверим, был ли сивол нуля и если надо установим это
			my->opt.min = my->tmp != 0 ? my->tmp : my->opt.min;//если после звезды есть число то нужно его сохранить
		}
		else
			my->opt.min = my->tmp;
	}
	else if (my->f[0] == '#')
		my->opt.b_prefix = 1;
	else if (fn_finish(my) == 0)
		return (0);
	return (1);
}

int			fn_display(register t_my *my)
{
	char			*p;

	ft_memset(&my->opt, 0, sizeof(t_option));
	while (my->f[0] != 0)
	{
		if (fn_flag(my) == 0)
			break ;
		my->f++;
	}
	if ((int)my->opt.min < 0)
	{
		my->opt.b_left = 1;
		my->opt.min = ~(int)my->opt.min + 1;// тупой printf  пришлось добавить...
	}
	if (my->opt.min >= INT_MAX || my->out + my->count >= INT_MAX - my->opt.min)//printf в MAC ограничивает на такое число //отнимаем так как мы не импользуем int
		return (0);
	my->format = my->f++[0];
	if (my->format == 0)//проверка если такого типа строка "%"
	{
		my->f--;
		return(1);
	}
	my->sp = (my->opt.b_null != 0 && my->opt.b_left == 0) ? 0x30 : 0x20;
	p = ft_strchr(my->stc->str_format, my->format);
	p = (p != 0) ? (char *)(p - my->stc->str_format + 1) : p;
	if ((int)my->opt.max < 0)
	{
		my->opt.b_max_over = my->opt.b_null == 0 ? 1 : my->opt.b_max_over;
		my->opt.b_max_over_str = 1;
		my->opt.max = my->opt.min;// тупой printf  пришлось добавить...
	}
	return ((my->stc->fn[(size_t)p])(my));
}