/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_init_variable_fun.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_init.h"

void		ft_42sh_init_variable_fun(register t_main_42sh *array)
{
	if ((array->pwd.path.buff = getcwd(0, 0)) == 0)//Получаем текущий путь из какого стартуем свою работу для ft_42sh_auto_create и для ft_42sh_exp_standart
		ft_42sh_exit(E_MEM_CODE_42SH, __FILE__, __func__, __LINE__);
	if ((array->b_location & LOCATION_STANDART_42SH) != 0)
	{
		if (ioctl(FD_TERMINAL_42SH, TIOCGPGRP, &array->pr.pid_main) == -1)//получим pid оболочки
			ft_42sh_exit(E_IOTL_CODE_42SH, __FILE__, __func__, __LINE__);
	}
	else
		array->pr.pid_main = array->pr.pid_fork;//получим pid оболочки
	if (ft_42sh_stub_ioctl(array, TIOCGWINSZ, &array->ws) == -1)//Получим размер терминала
		ft_42sh_exit(E_IOTL_CODE_42SH, __FILE__, __func__, __LINE__);
}
