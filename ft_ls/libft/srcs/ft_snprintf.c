/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:23:41 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/17 12:01:39 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf/inc/ft_printf.h"

int		ft_snprintf(char *str, size_t size, const char *format, ...)
{
	t_arg		arg;
	size_t		out;

	if (size > (size_t)INT_MAX + 1)
		return (-1);
	arg.f = format;
	arg.b = str;
	arg.size_b = size > 0 ? size - 1 : size;
	arg.b_in_str = 1;
	va_start(arg.ap, format);
	out = fn_big(&arg);
	va_end(arg.ap);
	return (out);
}