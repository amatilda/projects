/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_dq_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_dq.h"

void		ft_42sh_dq_cut(register t_main_42sh *array,
register t_in_42sh *list)
{
	list = ft_42sh_list_in_create_dup(array, list);
	ft_42sh_list_in_last(array);//Востановим значение конечного листа на дефолтные и заодно сделаем его текущим
	if (list->spl_slesh == 0)//Если нет - обнулим что бы проблем не было
		list->slesh_max = 0;
	array->dq.in_dquote = list;
	list->count_litter -= array->msg.pre_msg_litter;//Удаляем количество стартовых букв - потом могут измениться - что бы не было проблемм
}

void		ft_42sh_dq_paste(register t_main_42sh *array)
{
	register t_in_42sh		*last;
	register t_in_42sh		*prev;
	register t_in_42sh		*dquote;
	register size_t			tempos;

	dquote = array->dq.in_dquote;
	last = array->in.in_last;
	if ((tempos = (dquote->slesh_max + array->dq.slesh_alias)) != 0)
		ft_42sh_dq_split(dquote, tempos);//образования slesh
	dquote->lp_current = dquote->lp_b;
	dquote->count_litter_current = array->msg.pre_msg_litter;//Востановим количество стартовых букв
	dquote->count_litter += array->msg.pre_msg_litter;//Востановим количество стартовых букв
	prev = last->prev;
	if (prev != 0)
		prev->next = dquote;
	else
		array->in.in_first = dquote;//На случай если только два листа присутсвуют - dquote  и last
	last->prev = dquote;
	dquote->next = last;
	dquote->prev = prev;
}
