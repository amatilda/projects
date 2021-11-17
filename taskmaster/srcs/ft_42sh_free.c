/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

void		ft_42sh_free_script(register t_main_42sh *array)
{
	ft_42sh_auto_free_all_cmd_full(array);//Освобождаем все что связанно с всеми командами//Что бы валгринд не детектил утечки
	if (array->lp_spl_path != 0)//Предыдущий массив с путями к исполняемым файлам освобождаем//Что бы валгринд не детектил утечки
	{
		ft_strsplit_free(array->lp_spl_path);
		array->lp_spl_path = 0;
	}
	ft_free(array->pwd.path.buff);//текущий путь//Что бы валгринд не детектил утечки
	array->pwd.path.buff = 0;
	ft_free(array->pwd.prev_path.buff);//предыдущий путь//Что бы валгринд не детектил утечки
	array->pwd.prev_path.buff = 0;
	ft_free(array->pwd.path_view);//текущий путь но экранированый//Что бы валгринд не детектил утечки
	array->pwd.path_view = 0;
	ft_free(array->msg.hrdc);//Для хередока приветсмие//Что бы валгринд не детектил утечки
	array->msg.hrdc = 0;
	ft_42sh_jobs_free_all(array);//Чистим все jobs//Что бы валгринд не детектил утечки
	ft_42sh_alias_free(array);/// очистка алиасов //Что бы валгринд не детектил утечки
}

void		ft_42sh_free_exit(register t_main_42sh *array)
{
	ft_42sh_task_deinit(array);
	ft_42sh_free_script(array);
	ft_42sh_path_logins_list_free(array);//Что бы валгринд не детектил утечки
	ft_42sh_list_fun(&array->env.root, ft_free);//Освобождаем все что связанно с переменными среды//Что бы валгринд не детектил утечки
}
