/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h   	                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/17 12:06:23 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:24:38 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <wchar.h>
# include <stdarg.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

# define STR_FORMAT "sSpPdDioOuUxXcCnbBkfFeEaAr"
# define STR_SUB "hljtzL"
# define SIZE_B 0x4000//не меньше 0x2000 байт должен из-за float так проще

//fn_ftoa define;
# define PRINTF_MLT_DEC 	1000000000//
# define PRINTF_MLT_DEC_C	9//
# define PRINTF_SIZE_ROUND	19//
# define PRINTF_E			ITOA_LOWER | ITOA_SIGNED | ITOA_PLUS | ITOA_NOT_ONE//
# define PRINTF_P			ITOA_LOWER | ITOA_SIGNED | ITOA_PLUS//

typedef struct		s_float_mult//структура для удобства
{
	union
	{
		long double		value;
		struct
		{
			uint64_t	mantissa;
			uint16_t	exponent;
		};
	};
}					t_float_mult;

typedef struct		s_float//структура для float
{
	union
	{
		long double		value;
		struct
		{
			uint64_t	mantissa;
			uint16_t	exponent;
		};
	};
	char			*alphabet;//для перевода в ansii
	char			*start;//для округление указатель на первое число
	char			*end;//для округление указатель на текущее число
	char			*end_tmp;//для округление указатель на текущее число
	char			*first;//для округление указатель на место где будет первый вывод
	size_t			min;//храним времменно my->opt.min
	int16_t			e_save;//храним начальную експоненту числа для получения p+50 для шестнатиричного числа
	uint_fast16_t	float_fract_e;
	uint_fast16_t	float_sing_e;
	uint64_t		float_fract;
	uint64_t		float_sing;
	uint_fast8_t	up;//индикатор в верхнем или нижнем регистре буквы, если нижнем = 0
	uint_fast8_t	change;//используем для получения целой части дроби
	char			letter_max;
	char			letter_tmp;//просто временно храним букву
	char			letter_tmp2;//просто временно храним букву
	char			str[10];//максимум займут цифры, - не паримся максимальное число 16384 и e+
	char			buffer[3];//для вывода inf и nan
}					t_float;

typedef struct		s_option//данная структура обнуляеться всегда
{
	size_t			min;//Ширина
	size_t			max;//Точность
	uint_fast8_t	b_max_over;// когда max отрицательный влияет ноль или пробел использовать
	uint_fast8_t	b_max_over_str;// когда max отрицательный влияет используеться вся строка
	uint_fast8_t	b_plus;//+
	uint_fast8_t	b_grup;// '
	uint_fast8_t	b_left;// -
	uint_fast8_t	b_prefix;// #
	uint_fast8_t	b_space;// пробел
	uint_fast8_t	b_null;// 0
	uint_fast8_t	b_max;//указывает установлена ли точность
	uint_fast8_t	minus;//указывает eсли 0 то положительное иначе отрицательное
	uint_fast8_t 	base;//База для преобразования числа
	uint_fast8_t	b_sub[sizeof(STR_SUB) + 2];//для того что бы знать размер получаемых данных //доп флаги - ll,hh,h ...
	char			letter;//отвечает нужен знак перед числом или нет: "+", " ", "-" и так же хранит нужный символ
	uint_fast8_t	b_exp;//указывает используеться ли научная форма представления числа// начало онли FLOAT
	uint_fast16_t	grup;//храним количество пробелов
	uint_fast16_t	sing;//храним количество цифир в целой части и тем самым контролируем есть ли она у числа
	intmax_t		e;//если число без целлой части то здесь храним количество цифир для e+50
	uint_fast8_t	b_ox;//контролирует нужен перед числом "0x" а так же добавляет количество выводимых символов в общую кучу
	uint_fast8_t	count;//отвечает особое число или нет и так же хранит количество выводимых символов
	uint_fast8_t	b_round;//указвает сделано округление или нет
}					t_option;

typedef struct		s_arg//структура для передачи параметров
{
	size_t			size_b;
	size_t			fildes;
	const char		*restrict f;
	char			*restrict b;
	va_list 		ap;
	uint_fast8_t	b_in_str;
}					t_arg;

typedef struct		s_static
{
	char			*str_format;//ссылка на строку  флагов
	char			*str_sub;//ссылка на строку доп флагов - ll,hh,h ...
	char			*str_not[0x1F + 2];//таблица строк для не печатных
	int	 			(*fn[sizeof(STR_FORMAT)])();
}					t_static;

typedef struct		s_my
{
	size_t			end_count;//для тупого printf в солучае ошибки не надо выводить все что было после последней '%'  но так выводим все оптом и malloc не используем  - такой костыль для пиров
	size_t			size_b;//Размер буфера
	size_t			count;//текущее количество записанных байт
	size_t			out;//общее количество записанных байт
	size_t			tmp;//Временная переменная для чисел считываемых
	size_t			fildes;//буфер куда выводим, если не в строку
	size_t			arg;//текущае значение аргумента или не существующей команды как например %
	uintmax_t		arg_itoa;//текущае значение аргумента или не существующей команды как например %
	char			*str_tmp_sub;// временная для подсчета смещения
	const char		*restrict f;
	char			*restrict b;
	t_static		*stc;
	uint_fast8_t	null;//для определения ноль как число или символ заполнения
	uint_fast8_t	sp;//символ заполнения
	uint_fast8_t	format;// не существующей команды как например % или существующей как например S
	uint_fast8_t	b_in_str;//если 1 то выводим в строку
	uint_fast8_t	b_star;//если 1 то * уже были
	uint_fast8_t	b_point;//если 1 то точка была
	va_list 		ap;//Образец где хранить начало аргументов
	va_list 		ap_line;//копия для неприрывного в случае когда начинаетья новая группа $ с начала - сбрасываеться на начало аргументов
	va_list 		ap_n;//копия для того случая когда не нужно прерывать непрерывность
	t_float			flt;
	t_option		opt;
	char			buffer[SIZE_B];//буфер для ускарения
}					t_my;

//ft_str_time define;
# define STR_TIME_STANDART 	0x0//стандартное действие
typedef struct 					s_ft_str_time_in_out//структура для передачи параметров
{
	char *restrict 				s;
	const char *restrict 		f;
	struct tm 					timeptr;
	size_t						opt;
	size_t						max;
	size_t						count_out;
}								t_ft_str_time_in_out;

size_t				fn_big(register t_arg *arg);
int					fn_display(register t_my *my);
int					fn_display_float(register t_my *my);
int					fn_display_unknown(register t_my *my);
int					fn_display_float(t_my *my);
int					fn_display_s(register t_my *my);
int					fn_display_p(register t_my *my);
int					fn_display_d(register t_my *my);
int					fn_display_o(register t_my *my);
int					fn_display_u(register t_my *my);
int					fn_display_x(register t_my *my);
int					fn_display_c(register t_my *my);
int					fn_display_n(register t_my *my);
int					fn_display_b(register t_my *my);
int					fn_display_k(register t_my *my);
int					fn_display_f(register t_my *my);
int					fn_display_e(register t_my *my);
int					fn_display_a(register t_my *my);
int					fn_display_r(register t_my *my);
void				fn_sp(register t_my *my, register uint_fast8_t sp);
void				fn_str_copy(register t_my *my, register const char *str, register size_t count);
int					fn_str_time(register t_ft_str_time_in_out *in_out);
void 				fn_write_float(register t_my *my);

#endif
