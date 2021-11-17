/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_key_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_42sh_key.h"

static int				fn_test_env_path(register unsigned char *b,
register unsigned char *e)//Проверим может это дополнение переменной среды или путь к папке
{
	register unsigned char		litter;

	if ((litter = b[0]) == '$' || litter == '~' ||
	ft_42sh_parsing_path_test((void *)b, (void *)e) != 0)
		return (0);
	return (1);
}

static void				fn_spase_add(register t_main_42sh *array,
register t_in_42sh *list)
{
	t_add_litter_42sh			in;

	in.count = 4;
	in.count_litter = 4;
	ft_42sh_str(array, list, (void *)"    ", &in);
}

static void				*fn_pipe_end(register unsigned char *b,
register unsigned char *e)
{
	register unsigned char		litter;

	while (b < e && (litter = b[0]) != '<' && litter != '>')
		b++;
	return (b);
}

static void				*fn_set(register t_in_42sh *list,
unsigned char **start, unsigned char **end)
{
	register unsigned char		*b;

	*start = (void *)list->lp_b;
	*end = (void *)(*start + list->count);
	b = *start;
	return (b);
}

void					ft_42sh_key_tab(register t_main_42sh *array,
register t_in_42sh *list)
{
	register unsigned char		*b;
	register unsigned char		*e;
	unsigned char				*start;
	unsigned char				*end;

	b = fn_set(list, &start, &end);
	e = end;
	start = ft_42sh_key_tab_word_start(start, (void *)list->lp_current, end);
	end = ft_42sh_key_tab_word_end(start, end);
	b = ft_42sh_key_tab_cmd_start(b, start);//Учтем спецсимволы начало новой строки команд';' '|'
	e = ft_42sh_key_tab_cmd_end(end, e);//Учтем спецсимволы конца новой строки команд';' '|'
	if ((b = ft_42sh_parsing_sp(b, e)) == e)//Проверим может выведем все команды
		return (ft_42sh_auto_cmd(array, list, 0, 0));
	if (b > start)//Проверим может нужно пробелы добавить(вместо табуляции)
		return (fn_spase_add(array, list));
	if (b == start)
	{
		start = ft_42sh_key_tab_eq(start, end);
		if (b == start && fn_test_env_path(start, end) != 0)//Проверим команда или нет
			return (ft_42sh_auto_cmd(array, list, start, end - start));
	}
	else
		start = ft_42sh_key_tab_eq(start, end);
	ft_42sh_glb_auto_other(array, list, (void*)start,
	fn_pipe_end(start, end));
}
