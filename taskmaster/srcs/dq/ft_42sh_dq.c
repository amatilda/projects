/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_dq.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_42sh_dq.h"

static unsigned char	fn_combi(register t_main_42sh *array,
register t_in_42sh *list, register unsigned char litter)
{
	register t_in_42sh			*parser;
	register void				*tmp;

	parser = array->dq.in_dquote;
	ft_42sh_dq_combi_date(array, parser, list);//Соединим старые с новыми данными
	tmp = parser->lp_b;
	if ((litter = ft_42sh_dq_test(array, parser, tmp,
	tmp + parser->count)) == 0 || litter == PARSING_FALTURE_42SH)
		ft_42sh_dq_paste(array);
	return (litter);
}

static void				fn_finish(register t_main_42sh *array)
{
	ft_42sh_list_in_last(array);//Востановим значение конечного листа на дефолтные и заодно сделаем его текущим
	ft_42sh_list_in_dup_restore(array);
	ft_write_buffer_str_zero(&array->out, array->msg.pre_msg);
	ft_write_buffer_str_zero(&array->out, " \x1b[1D");//Для того что бы перевод строки авто был вставляем лишний пробел и потом смещаем вправо сразу что бы не было видно
}

void					ft_42sh_dq(register t_main_42sh *array,
register t_in_42sh *list)
{
	register void				*tmp;
	register unsigned char		litter;

	ft_42sh_str_full_end(array, list);//Сначала сместим каретку в саммый коне прежде чем будем выводить что либо
	ft_write_buffer_str_zero(&array->out, "\n\x1b[J");//Стартуем стандартную начальное отображение прежде чем выводить что либо почистим экран сначала
	if ((litter = array->dq.dquote) != 0)
		litter = fn_combi(array, list, litter);
	else
	{
		tmp = list->lp_b;
		if ((litter = ft_42sh_dq_test(array, list, tmp,
		tmp + list->count)) != 0 && litter != PARSING_FALTURE_42SH)
			ft_42sh_dq_cut(array, list);//Вырежим лист в котором был включен данный режим
	}
	ft_42sh_msg_change(array, (litter == PARSING_FALTURE_42SH ? 0 : litter),
	array->dq.dquote);//Изменим стартовый текс и изменим длину стартовых букв во всех листах
	if (litter == 0 || litter == PARSING_FALTURE_42SH)
		return (ft_42sh_parsing(array, list, litter));//С данного режима вышли - запускаем парсинг команд
	fn_finish(array);
}
