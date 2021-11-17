/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/17 12:06:23 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:24:38 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdint.h>
# include <stddef.h>
# include <time.h>

//ft_printf define;
# define PRT_RESET   	"\x1b[0m"
# define PRT_RED     	"\x1b[31m"
# define PRT_GREEN   	"\x1b[32m"
# define PRT_YELLOW  	"\x1b[33m"
# define PRT_BLUE 		"\x1b[34m"
# define PRT_PURPLE  	"\x1b[35m"
# define PRT_LIGHT_BLUE "\x1b[36m"
# define PRT_WHITE   	"\x1b[37m"

//ft_itoa define;
# define ITOA_LOWER  	0x0//строчные буквы
# define ITOA_UPPER  	0x1//прописные буквы
# define ITOA_SIGNED 	0x2//считает что получает базу с  отрицательными числами, если отрицательно ставит перед -
# define ITOA_GROUPED 	0x4//групирует по три символа и разделяет пробелом
# define ITOA_PLUS 		0x8//если установлен ITOA_SIGNED но число положительное то перед ним ставит +
# define ITOA_NOT_ONE	0x10//если установлен то к одному числу добавляеться ноль: 01

//Standart functions
int			ft_strcmp(const char *s1, const char *s2);
size_t		ft_strlen(const char *s);
size_t		ft_wcslen(const wchar_t *s);
char		*ft_strdup(const char *s1);
char		*ft_strndup(const char *s1, size_t n);
char		*ft_strchr(const char *s, int c);
int			ft_printf(const char *format, ...);
int			ft_snprintf(char *str, size_t size, const char *format, ...);
void		*ft_memset(void *b, int c, size_t len);
int			ft_atoi(const char *str);
int			ft_itoa(char *buffer, uintmax_t value, uint_fast8_t base, uint_fast8_t b_up);
int			ft_wctomb(char *s, wchar_t wchar);
struct tm	*ft_gmtime_r(const time_t *clock, struct tm *result);

#endif
