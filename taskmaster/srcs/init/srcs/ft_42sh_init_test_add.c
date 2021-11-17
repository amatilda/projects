/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_init_test_add.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_init.h"

static void		fn_signal_add(int signo)
{
	register t_main_42sh			*array;

	array = g_lp_array;
	array->count_tty_out_in++;
	(void)signo;
}

static void		fn_signal_quit(int signo)
{
	exit(0);
	(void)signo;
}

static void		fn_error(void)
{
	write(STDERR_FILENO, "\x56\x61\x6C\x67\x72\x69\x6E\x64\x20\x6E\x6F\x74"
	"\x20\x73\x75\x70\x70\x6F\x72\x74\n", 21);
	exit(0);
}

static pid_t	fn_fork_child(void)
{
	register pid_t				pid;

	if ((pid = fork()) == 0)
	{
		while (0xFF)
			;//В вечный цикл  ждем покуда нас убьют
	}
	else if (pid < 0)
	{
		write(STDERR_FILENO, WAR_PR_42SH""MSG_FOOR_TXT_42SH""PRTF_RESET,
		sizeof(WAR_PR_42SH""MSG_FOOR_TXT_42SH""PRTF_RESET) - 1);
		exit(E_FOOR_CODE_42SH);
	}
	return (pid);
}

void			ft_42sh_init_test_add(register t_main_42sh *array)
{
	register pid_t				pid;
	int							status;

	if ((array->b_mode & MODE_VALG_42SH) == 0)//Проверяем какой режим работы шела
		return ;
	signal(SIGQUIT, fn_signal_quit);//что бы валгринд не сообщал об том что сигнал сработал перехватим его и просто выйдем нормально с процесса
	signal(SIGCHLD, fn_signal_add);//данный сигнал перехватывает валгринд поэтому если у нас не вызовиться он значит пад валгриндам
	array->count_tty_out_in = 0;//обнулим что бы знать
	pid = fn_fork_child();//Создадим процесс для проверки
	kill(pid, SIGQUIT);//Завершаем работу процесса
	waitpid(pid, &status, WUNTRACED);//проверяем что он завершился
	if (array->count_tty_out_in == 0)//Если в SIGCHLD не попадали значит мы под валгриндам
		fn_error();
	signal(SIGQUIT, SIG_DFL);//востаноми сигналы на значения по умолчанию
	signal(SIGCHLD, SIG_DFL);//востаноми сигналы на значения по умолчанию
}
