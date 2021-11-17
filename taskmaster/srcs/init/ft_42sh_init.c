/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_42sh_init.h"

void			ft_42sh_init(register t_main_42sh *array, register int argc,
register char **argv, register char **env)
{
	ft_42sh_init_test(array);//Для правильного останова когда в фоне запускают нас
	ft_42sh_init_variable(array, LOCATION_STANDART_42SH);//Установим некоторые значения
	ft_42sh_init_terminal(array);//Иницилизируем терминал
	ft_42sh_init_test_while(array);//установим зацикливание что бы отлавливать все негативные результаты и востанавливать терминал
	ft_42sh_signal_initilization(array);//Установим сигналы
	ft_42sh_init_fd();//Иницилизируем fd
	ft_42sh_init_event(array);//Иницилизируем для синхронизации различной
	ft_42sh_init_variable_fun(array);//Иницилизируем переменные с помощью системных функций
	ft_crc32_tabl_mirror(array->lp_crc32_mirror, CRC32_DEPTH_42SH,
	CRC32_POLINOM_MIRROR_42SH);
	ft_42sh_list_in_open(array);
	ft_42sh_alias_pars_home(array);///парсинг домашней директории для работы с алиасами
	ft_42sh_alias_read_file(array, 0);/// считывание алиасов из файла
	ft_42sh_path_logins(array);//Создадим листы с логинами пользователей и путями к ним
	ft_42sh_exp_start(array, env);//Составим список листов переменнх сред
	ft_42sh_task_init(array, argc, argv);
}

void			ft_42sh_init_script(register t_main_42sh *array,
register char **env, register char **lp_arg)
{
	ft_42sh_free_script(array);//Освоболдим  ресурсы которые могут вызвать конфликт
	ft_42sh_task_deinit(array);//Специально - что бы путь хоме убрался и файл не сохраняло
	ft_42sh_init_variable(array, LOCATION_SCRIPT_42SH);//Установим некоторые значения
	ft_42sh_signal_initilization_script(array);
	ft_42sh_init_fd();//Иницилизируем fd
	ft_42sh_init_event(array);//Иницилизируем для синхронизации различной
	ft_42sh_init_variable_fun(array);//Иницилизируем переменные с помощью системных функций
	ft_42sh_init_script_env(array, env, lp_arg);
}
