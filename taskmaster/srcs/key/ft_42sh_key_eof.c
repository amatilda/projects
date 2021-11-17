/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_key_eof.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

static void		fn_check_list(register t_main_42sh *array,
register t_in_42sh *list)
{
	register void				*cmp;

	cmp = array->dq.hrdoc_cmp_lp;
	ft_42sh_str_add(array, cmp, cmp + array->dq.hrdoc_cmp_count, 1);//+1 //Уже опрботанно поэтому тут нет лишнего пробела с переводос строки
	ft_free(cmp);
	array->dq.hrdoc_cmp_lp = 0;
	return (ft_42sh_dq(array, list));
}

void			ft_42sh_key_eof(register t_main_42sh *array,
register t_in_42sh *list)
{
	register size_t				count;

	if ((count = list->count) == 0)
	{
		if ((count = array->dq.dquote) == 0)
		{
			ft_write_buffer_str_zero(&array->out, "\n");
			ft_42sh_cm_exit(array, 0);//При ctr + D - выходим так как можем получить данную команду если не запущен другой процесс
			return ;//Если есть jobs не выходим поэтому и надо
		}
		else if (count == '<')//Изза чекслиста -в zsh так heredoc не работает если heredoc
			return (fn_check_list(array, list));
		else
			return (ft_write_buffer_str_zero(&array->out, "\x7"));//Звуки
	}
	if (list->lp_b + count == list->lp_current)
		return (ft_write_buffer_str_zero(&array->out, "\x7"));//Звуки
	ft_42sh_key_delete(array, list);
}
