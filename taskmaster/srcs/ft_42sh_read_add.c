/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_read_add.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

static void		fn_test_key_finish(register t_main_42sh *array,
register t_in_42sh *list, register uintmax_t litter)
{
	if (litter == KEY_CLEAR_42SH)//Чистим весь экран
		ft_42sh_key_clear(array, list);
	else if (litter == KEY_WORD_LEFT_42SH)//Перемещаем на слово влево
		ft_42sh_key_word_left(array, list);
	else if (litter == KEY_WORD_RIGHT_42SH)//Перемещаем на слово вправо
		ft_42sh_key_word_right(array, list);
	else if (litter == KEY_HOME_FULL_42SH)//Перемещаем в самое начало
		ft_42sh_str_full_home(array, list);
	else if (litter == KEY_END_FULL_42SH)//Перемещаем в самый конец
		ft_42sh_str_full_end(array, list);
}

static void		fn_test_key_add(register t_main_42sh *array,
register t_in_42sh *list, register uintmax_t litter, register size_t count)
{
	ft_42sh_dsp_clear_select(array, list);
	if (ft_42sh_str_test_read(array, list, (void *)array->litter, count) != 0)
		return ;
	if (litter == KEY_INT_42SH)//Удаляем букву находящуюся справа от каретки
		ft_42sh_key_int(array, list);
	else if (litter == KEY_ENTER_42SH || litter == KEY_ENTER2_42SH)//Выполняем все что пользователь в вел
		ft_42sh_dq(array, list);
	else if (litter == KEY_UP_42SH)//Перебираем придыдущие варианты если есть они - вверх
		ft_42sh_key_up(array, list);
	else if (litter == KEY_DOWN_42SH)//Перебираем придыдущие варианты если есть они - вниз
		ft_42sh_key_down(array, list);
	else if (litter == KEY_RIGHT_42SH)//Перемещаем каретку в право на букву
		ft_42sh_key_right(array, list);
	else if (litter == KEY_LEFT_42SH)//Перемещаем каретку в влево на букву
		ft_42sh_key_left(array, list);
	else if (litter == KEY_HOME_42SH)//Перемещаем каретку в начало строки
		ft_42sh_key_home(array, list);
	else if (litter == KEY_END_42SH)//Перемещаем каретку в конец строки
		ft_42sh_key_end(array, list);
	else if (litter == KEY_EOF_42SH)//ctr + D
		ft_42sh_key_eof(array, list);
	else
		fn_test_key_finish(array, list, litter);
}

static void		fn_test_key(register t_main_42sh *array,
register t_in_42sh *list, register uintmax_t litter, register size_t count)
{
	ft_42sh_dsp_clear_auto(array, list);
	ft_42sh_auto_toogle(array);//Если надо переключим авто - дополнение
	if (litter == KEY_SELECT_LEFT_42SH)
		ft_42sh_select_left(array, list);//Выделяем в лево
	else if (litter == KEY_SELECT_RIGHT_42SH)
		ft_42sh_select_right(array, list);//Выделяем в право
	else if (litter == KEY_STR_DELETE_42SH)
		ft_42sh_key_str_delete(array, list);//Удаляем всю строку или выделеное
	else if (litter == KEY_STR_COPY_42SH)//Копируем в локальный буфер выделенное или всю строку
		ft_42sh_key_str_copy(array, list);
	else if (litter == KEY_STR_PASTE_42SH)//Вставляем из локального буфера выделенное или всю строку
		ft_42sh_key_str_paste(array, list);
	else if (litter == KEY_STR_CUT_42SH)//Вырезаем выделенное или всю строку
		ft_42sh_key_str_cut(array, list);
	else if (litter == KEY_DELETE_42SH)//Удаляем букву находящуюся справа от каретки
		ft_42sh_key_delete(array, list);
	else if (litter == KEY_BACKSPACE_42SH)//Удаляем букву находящуюся слева от каретки
		ft_42sh_key_backspace(array, list);
	else
		fn_test_key_add(array, list, litter, count);
}

static void		fn_test_key_pre(register t_main_42sh *array,
register t_in_42sh *list, register uintmax_t litter, register size_t count)
{
	if (array->b_quest_exit != 0)
		ft_42sh_jobs_exit_clear(array, list, litter);
	else if (litter == KEY_TAB_42SH && array->lp_auto->b_auto != 0 &&
	array->lp_auto->b_view == 0)
		ft_42sh_auto_dsp_next_cmd(array, list);
	else if (array->lp_auto->b_limit != 0 || litter == KEY_TAB_42SH)//Проверяем надо ли показать варианты команды
		ft_42sh_key_tab(array, list);
	else
		fn_test_key(array, list, litter, count);
}

void			ft_42sh_read_finish(register t_main_42sh *array,
register size_t count)
{
	if (count == 0)
		return ;
	fn_test_key_pre(array, array->in.in_current, array->litter[0], count);
	ft_write_buffer(&array->out);//Записываем все команды что на собирались
}
