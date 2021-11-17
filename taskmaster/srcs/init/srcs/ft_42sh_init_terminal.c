/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_init_terminal.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_init.h"

void		ft_42sh_init_terminal(register t_main_42sh *array)
{
	dup2(STDIN_FILENO, FD_TERMINAL_42SH);//Сделаем себе fd терминала для технических работ
	if (ioctl(FD_TERMINAL_42SH, TIOCGETA, &array->tty_change) == -1)//Получим состояние терминала
	{
		write(STDERR_FILENO, CRIT_PR_42SH""E_CANON_TXT_42SH""PRTF_RESET,
		sizeof(CRIT_PR_42SH""E_CANON_TXT_42SH""PRTF_RESET) - 1);//Если не терминал то не работаем - не так буде как надо работать
		exit(E_CANNON_CODE_42SH);
	}
	ft_memcpy(&array->tty, &array->tty_change, sizeof(array->tty_change));//сохраняем для последующего востановления
	array->tty_change.c_lflag &= ~(ICANON | ECHO | ISIG);//переключаем в не конанический режим //Запрещаем выводить символы на экран ISIG - блокируес ctr + C ctr + \ ctr + Z
	array->tty_change.c_cc[VMIN] = 1;//Покуда не считает хоть один файл read не заканчивает свою работу
	array->tty_change.c_cc[VTIME] = 0;//Отменяем ограничения по времени на ожидание
	if (ioctl(FD_TERMINAL_42SH, TIOCSETA, &array->tty_change) == -1)//Изменили состояние терминала
	{
		write(STDERR_FILENO, CRIT_PR_42SH""E_CANON_TXT_42SH""PRTF_RESET,
		sizeof(CRIT_PR_42SH""E_CANON_TXT_42SH""PRTF_RESET) - 1);
		exit(E_CANNON_CODE_42SH);
	}
}
