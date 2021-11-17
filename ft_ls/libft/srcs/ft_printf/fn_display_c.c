/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_display_c.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_printf.h"

int	fn_display_c(register t_my *my)
{
	char	str[5];// 4 байта максимум размер вывадимой многайтной строки которую можно получить

	my->arg = (wint_t)va_arg(my->ap_line, size_t);
	if (my->opt.b_sub[1] == 0 && my->format != 'C')
	{
		if (my->out + my->count >= INT_MAX - 1)//printf в MAC ограничивает на такое число - минус один на планируемое записать число
			return (0);
		my->format = (unsigned char)my->arg;
		fn_display_unknown(my);
		return (1);
	}
	my->opt.max = ft_wctomb(str, my->arg);
	if ((int)my->opt.max == -1)
		return (0);
	if (my->out + my->count >= INT_MAX - my->opt.max)//printf в MAC ограничивает на такое число - my->opt.max на планируемое записать число
		return (0);
	if (my->opt.b_left != 0)
		fn_str_copy(my, str, my->opt.max);
	if (my->opt.min-- >= 2)
		fn_sp(my, my->sp);
	if (my->opt.b_left == 0)
		fn_str_copy(my, str, my->opt.max);
	return (1);
}