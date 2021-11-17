/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_key_sub_litter.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

void				ft_42sh_key_backspace(register t_main_42sh *array,
register t_in_42sh *list)
{
	register char		*current;
	register char		*offset;
	register char		litter;

	if (array->slc.b != 0)
		return (ft_42sh_key_str_delete(array, list));
	if (list->lp_b == (current = list->lp_current))//Что бы не ути за левую границу
		return ;
	offset = current;
	while (((litter = offset--[-1]) & 0x80) != 0 && (litter & 0x40) == 0)//Проверяем utf-8 что бы знать на сколько байт сместить
		;
	if (litter == '\n')
		ft_42sh_str_delete(array, list, offset - 1, 0);
	else
		ft_42sh_str_delete(array, list, offset, 1);
}

void				ft_42sh_key_delete(register t_main_42sh *array,
register t_in_42sh *list)
{
	register char		*current;
	register char		*end;
	register char		*offset;
	register char		litter;

	if (array->slc.b != 0)
		return (ft_42sh_key_str_delete(array, list));
	end = list->lp_b + list->count;
	if (end == (current = list->lp_current))//Что бы не ути за правую границу
		return ;
	offset = current;
	if (((litter = offset++[0]) & 0x80) != 0)
		while (((litter = litter << 1) & 0x80) != 0)
			offset++;
	if (end != offset && offset[0] == '\n')//Что бы не ути за правую границу
		ft_42sh_str_delete(array, list, offset + 1, 0);
	else
		ft_42sh_str_delete(array, list, offset, 1);
}
