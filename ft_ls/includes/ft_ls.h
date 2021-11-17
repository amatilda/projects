/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/17 12:06:23 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:24:38 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/acl.h>
# include <sys/xattr.h>
# include <grp.h>
# include <pwd.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <string.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>
# include <sys/stat.h>

# define NAME_PROG					"ls"
# define FLAG_A						'A'// -A -- все кроме . и ..
# define FLAG_F						'F'// -F -- после папок '/', испольняемых '*' ....
# define FLAG_RECURS				'R'// -R -- обход всех каталогов
# define FLAG_SORT_SIZE				'S'// -S -- сортеровка по размеру
# define FLAG_FULL_DATE				'T'// -S -- выводит в режиме 'l' полную дату
# define FLAG_SORT_TIME_CREATION	'U'// -U -вместо st_mtimespec используем st_birthtimespec в -l и -t
# define FLAG_SYSTEM				'a'// -a -- отображает также и скрытые файлы системные - файлы начинающиеся на точку
# define FLAG_SORT_LAST_CHANGE		'c'// -c вместо st_mtimespec используем st_ctimespec в -l и -t
# define FLAG_ONLY_DIR				'd'// -d отображает только папки без рекурсии
# define FLAG_ONLY_GROOP			'g'// -g -- форсирует -l и отображает только группу владельца
# define FLAG_NOT_SORT				'f'// -f -- без сортировки
# define FLAG_INFO_FULL				'l'// -l -- отображает подробную информацию
# define FLAG_ONLY_USER				'o'// -o -- форсирует -l и отображает только владельца
# define FLAG_DIR_SLECH				'p'// -l -- после папоп отображает '/'
# define FLAG_REVERSE				'r'// -r -- обращает текущую сортировку
# define FLAG_LAST_MODIF			't'// -t --сортируем по времени последней модификации
# define FLAG_SORT_TIME_ACCES		'u'// -u -вместо st_mtimespec используем st_atimespec в -l и -t
# define FLAG_ONE_LINE				'1'// -1 -- отобразит все в один столбик кроме когда параметр -l
# define FT_LS_BUFFER_SIZE			0x4000
# define FT_LS_BUFFER_SIZE_INVALID	0x1000
# define FT_LS_MAX_NAME				0xFF + 1
# define STR_OPTION		"AFRSTUacdgfloprtu1"
# define USAGE_PRE		NAME_PROG": illegal option -- "
# define USAGE_LAST		"\nusage: "NAME_PROG" [-"STR_OPTION"] [file ...]\n"
# define ERROR_NULL_STR	NAME_PROG": fts_open"

typedef struct				s_list_type//структура для удобства
{
	union
	{
		size_t				tempos;
		struct
		{
			char			buff;
			char			type;
		};
	};
}							t_list_type;

typedef struct				s_list_invalid//все что нельзя открыть
{
	char					*name;//имя чтор не льзя открыть
	struct s_list_invalid	*prev;
	struct s_list_invalid	*next;//первые три параметра должны быть у всех листов и в одинаковом порядке идти
	int						errno_;//номер ошибки при попытки открыть
	mode_t					st_mode_type;
}							t_list_invalid;

typedef struct				s_list_file
{
	char					*name;//имя файла
	struct s_list_file		*prev;
	struct s_list_file		*next;//первые три параметра должны быть у всех листов и в одинаковом порядке идти
	struct timespec			st_timespec;//time of last data modification
	char					*patch;//имя файла
	char					*pw_name;//должно быть зануленно имя владельца
	char					*gr_name;//должно быть зануленно имя группы владецьца
	size_t					pw_size;//Длина имени владельца
	size_t					gr_size;//Длина мени группы
	mode_t					st_mode;//Определяем права на чиение запись
	mode_t					st_mode_type;// Тип файла - файл папка ....
	off_t					size_file;//Размер
	nlink_t					st_nlink;//Количестов ссылок
	dev_t					st_rdev;//В случае устройства используеться вместо size_file
}							t_list_file;

typedef struct				s_list_dir
{
	char					*name;//имя папки
	struct s_list_dir		*prev;
	struct s_list_dir		*next;//первые три параметра должны быть у всех листов и в одинаковом порядке идти
	struct timespec			st_timespec;//time of last data modification
	off_t					size_file;//Дублируем так как папки тоже по размеру нужнл сортировать
}							t_list_dir;

typedef struct				s_fl_ls_list_invalid
{
	t_list_invalid			*first;//должно быть зануленно
	t_list_invalid			*center;
	t_list_invalid			*last;//первые три параметра должны быть у всех листов и в одинаковом порядке идти
}							t_fl_ls_list_invalid;

typedef struct				s_fl_ls_list_dir
{
	t_list_dir				*first;//должно быть зануленно
	t_list_dir				*center;
	t_list_dir				*last;//первые три параметра должны быть у всех листов и в одинаковом порядке идти
	size_t					count;//должно быть зануленно
}							t_fl_ls_list_dir;

typedef struct				s_fl_ls_list_file
{
	t_list_file				*first;//должно быть зануленно   //первый элемент
	t_list_file				*center;//центральный относительно которого добавляем слева или вправо
	t_list_file				*last;// последний элемент //первые три параметра должны быть у всех листов и в одинаковом порядке идти
	t_list_file				*list_tmp;//должно быть зануленно
	off_t					max_size;//должно быть зануленно максимальный возможный размер файла для коретноного отображения нужного количества пробеллов
	blkcnt_t				st_blocks;//должно быть зануленно количество болоков занимаемое на диске для папки береться сумма всех блоков файлов
	nlink_t					max_nlink;//должно быть зануленно максимальный возможное количество ссылок для коретноного отображения нужного количества пробеллов
	size_t					max_pw_name;//должно быть зануленно максимальный возможный размер имени для коретноного отображения нужного количества пробеллов
	size_t					max_gr_name;//должно быть зануленно максимальный возможный размер группы для коретноного отображения нужного количества пробеллов
}							t_fl_ls_list_file;

typedef struct				s_main
{
	struct timespec			*st_timespec;//указатель на получения нужное время с файла
	char					option[0x7F];//должно быть зануленно массив для определения флагов по кодирвке ansii
	t_fl_ls_list_invalid	invalid;//Нужно для папок и файлов поданных через командную строку
	t_fl_ls_list_file		file;//Нужно для папок и файлов поданных через командную строку
	t_fl_ls_list_invalid	*inv_new;//Изза ограничения норминета так передаем в функцию нудное количество параметров
	t_fl_ls_list_file		*file_new;//Изза ограничения норминета так передаем в функцию нудное количество параметров
	t_fl_ls_list_dir		*dir_new;//Изза ограничения норминета так передаем в функцию нудное количество параметров
	struct dirent			*dp;
	char					*join;//должно быть зануленно //Изза ограничения норминета так передаем в функцию полный путь к файлу(выделяеться динамически)
	int						(*f)();//храним lstat или stat в случае 'l' - lstat, '1' - stat -  для коректной обработки в лучае символьных ссылок
	char					*err_str;// В случае "ls a '' b" если имя файла пустое то что выводим в виде ошибки
	char					type;//изза ограничений сохраняем в массив тип фала на конце что бы если надо вывестти
	time_t					offset_sec;//Смещение в секундах относительно гринвича
	time_t					current_moth;//Текущий месяц для определения давности файла  и нужно год выводить или время
	time_t					current_year;//Текущий год для определения давности файла  и нужно год выводить или время
	size_t					count;//указывает сколько занято в буфере array->b должно быть зануленно
	size_t					count_invalid;//указывает сколько занято в буфере array->b_invalid должно быть зануленно
	uint_fast8_t			b_level;//должно быть зануленно
	struct stat				stat_info;//Так как однапоточная программа то храним тут для удобства результат lstat/stat
	char					out_pre[FT_LS_MAX_NAME];//Времменый буфер для той же Itoa
	char					b[FT_LS_BUFFER_SIZE];//буфер для вывода текста
	char					b_invalid[FT_LS_BUFFER_SIZE_INVALID];//буфер для вывода текста ошибок
}							t_main;

int							ft_ls_list_add_invalid(register t_main *array,
	register t_fl_ls_list_invalid *invalid, register char *name);
int							ft_ls_list_add_dir(register t_main *array,
	register t_fl_ls_list_dir *dir, char *name);
int							ft_ls_list_add_file(register t_main *array,
	register t_fl_ls_list_file *file, char *name);
void						ft_ls_display(register t_main *array,
	t_fl_ls_list_invalid *invalid, t_fl_ls_list_file *file,
	uint_fast8_t b_level);
void						ft_ls_display_reverse(register t_main *array,
	register char *list_name);
int							ft_ls_recursive(register t_main *array,
	register t_fl_ls_list_dir *dir, uint_fast8_t b_level);
int							ft_ls_free(register t_main *array,
	register t_list_invalid *invalid, register t_list_file *file,
	register t_list_dir *dir);
ssize_t						ft_ls_sort(register t_main *array,
	register char *root, register char *list,
	ssize_t (*cmp)(register t_main *,
		register char *, register char *));
ssize_t						ft_ls_strcmp_name(register t_main *array,
	register char *root, register char *list);
ssize_t						ft_ls_strcmp_invalid(register t_main *array,
	register char *curent, register char *list);
ssize_t						ft_ls_strcmp_last_modif(register t_main *array,
	register char *curent, register char *list);
ssize_t						ft_ls_strcmp_size(register t_main *array,
	register char *curent, register char *list);
void						fn_dub_char(register t_main *array,
	register const char litter, register size_t dup_count);
void						fn_copy_str_zero(register t_main *array,
	register const char *str);
void						fn_copy_str(register t_main *array,
	register const char *str, register size_t n);
void						fn_copy_str_zero_invalid(register t_main *array,
	register const char *str);
void						fl_ls_add_list_init(register t_main *array,
	t_fl_ls_list_invalid *inv_new, register t_fl_ls_list_file *file_new,
	t_fl_ls_list_dir *dir_new);
void						ft_ls_init_main0(register t_main *array);
void						ft_ls_init_main1(register t_main *array,
	register t_fl_ls_list_dir *dir);
void						fl_ls_add_list_test(register t_main *array,
	register char *list_name, uint_fast8_t b_level);
void						fn_display_info_full(register t_main *array,
	t_fl_ls_list_file *file, register t_list_file *list, uint_fast8_t b_level);
void						ft_ls_display_full_add(register t_main *array,
	t_fl_ls_list_file *file, register t_list_file *list, register char *buff);

#endif
