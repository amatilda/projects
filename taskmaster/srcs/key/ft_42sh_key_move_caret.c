/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_key_move_caret.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

size_t				ft_42sh_key_left(register t_main_42sh *array,
register t_in_42sh *list)
{
	register char				*current;
	register char				litter;

	current = list->lp_current;//Каректрируем так как Показывает на адрес для будущей записи
	if (current == list->lp_b)//Что бы не ути за левую границу
		return (0);
	while (((litter = current--[-1]) & 0x80) != 0 && (litter & 0x40) == 0)//Учитываем utf-8 что бы знать на сколько байт сместить в лево
		;
	if (litter == '\n')
		ft_42sh_dsp_caret_left(array, list, 0, list->lp_current - current + 1);
	else
		ft_42sh_dsp_caret_left(array, list, 1, list->lp_current - current);
	return (1);
}

size_t				ft_42sh_key_right(register t_main_42sh *array,
register t_in_42sh *list)
{
	register char				*current;
	register char				litter;
	register char				lit;

	current = list->lp_current;
	if (list->lp_b + list->count == current)//Что бы не ути за правую границу
		return (0);
	litter = current++[0];
	if (((lit = litter) & 0x80) != 0)//Учитываем utf-8 что бы знать на сколько байт сместить в право
		while (((lit = lit << 1) & 0x80) != 0)
			current++;
	if (litter == ' ' && list->lp_b + list->count != current &&
	current[0] == '\n')
		ft_42sh_dsp_caret_right(array, list, 0, current - list->lp_current + 1);
	else
		ft_42sh_dsp_caret_right(array, list, 1, current - list->lp_current);
	return (1);
}
