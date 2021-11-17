/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_pguitar_struct.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguitar <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 16:12:24 by pguitar           #+#    #+#             */
/*   Updated: 2020/01/19 16:12:28 by pguitar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_42SH_PGUITAR_STRUCT_H
# define FT_42SH_PGUITAR_STRUCT_H

typedef struct					s_pguitar_comp_42sh
{
	char						*cur;
	char						*str;
	char						*dirr;
	char						*open_dirr;
	char						*str_dup;
	char						*dollar;
	char						*cur_dol;
	char						*read_dir;
	size_t						del;
	size_t						count_read;
	char						*caret;//Позволяет определить надо расширять переменную среды или делать перебор, Если указывает на текущий позицию каретки во время расшерения и если не отображали перебор то расширяем
	size_t						figure;
	size_t						type_q;
	size_t						count_str;
	size_t						count_cur;
	size_t						count_cur_litter;
	size_t						count_dirr;
	size_t						count_d;
	size_t						dir_dol;///!!! если '/' перед $
	size_t						*root;
	size_t						slash;
}								t_pguitar_comp_42sh;

typedef struct					s_pguitar_alias_42sh//Стандартная структрура для листов на основе ключей - ключи используються для поиска - вначале должна быть
{
	char						*name;//имя alias
	char						*value;//значения alias
	size_t						n_cnt;//name_count длина name в байтах
	size_t						v_cnt;//name_count длина value в байтах
	size_t						n_quote;//проверяет нужны ли для name кавычки
	size_t						v_quote;//проверяет нужны ли для value кавычки
	size_t						new_line;//количество переводов строки в value
	struct s_pguitar_alias_42sh	*next;
	char						lp[1];//указатель на конец структуры
}								t_pguitar_alias_42sh;

typedef struct					s_pguitar_alias_file_42sh
{
	size_t						crc32;//Детектил он не инцелезированную память справедлива просто это было из-за выравнивания к uint32_t добавлялось еще 4 байта неинцелезированных
	size_t						offset_alias;
	size_t						size_alias;
	char						lp[1];
}								t_pguitar_alias_file_42sh;

typedef struct					s_pguitar_alias_modif_42sh
{
	char						*home;
	size_t						modif;///должно быть занулено
	size_t						file;///должно быть занулено
	size_t						del_file;
	size_t						new_line;//количество переводов строки в value
	t_pguitar_alias_42sh		*first_list;
}								t_pguitar_alias_modif_42sh;

typedef struct					s_pguitar_42sh//Все структуры - pguitar
{
	t_pguitar_comp_42sh			comp;
	t_pguitar_alias_modif_42sh	f_modif;
	t_pguitar_alias_42sh		*list;
}								t_pguitar_42sh;

#endif
