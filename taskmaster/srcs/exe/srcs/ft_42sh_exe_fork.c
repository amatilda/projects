/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_exe_fork.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_exe.h"

void			ft_42sh_exe_fork(register t_main_42sh *array,
register t_jobs_42sh *jobs, int *fds)//Подготавливает для ft_42sh_pipe_left ft_42sh_pipe_right и бултин в форке и обычные программы
{
	register pid_t				pid;

	array->b_location |= LOCATION_FORK_42SH;//Помечаем что находимся в fork
	if (read(fds[PIPE_READ_42SH], &array->pr.pid_fork, sizeof(pid_t)) == -1)//Раз извращаться то извращаться используем вместо getpid() передачу pid через папй
		ft_42sh_exit(E_PIPE_CODE_42SH, __FILE__, __func__, __LINE__);
	ft_42sh_signal_default(array, jobs);
	close(fds[PIPE_READ_42SH]);
	close(fds[PIPE_WRITE_42SH]);
	if (ft_42sh_pipe_left(array, jobs) == 0 ||
	ft_42sh_pipe_right(array, jobs) == 0)
	{
		ft_42sh_dsp_err_msg(array, WAR_PR_42SH""MSG_FOOR_TXT_42SH""PRTF_RESET);
		exit(STATUS_FALTURE_42SH);
	}
	if ((array->b_mode & MODE_SIGCHILD_42SH) != 0)//Проверяем какой режим работы шела
	{//после группы ft_42sh_pipe_left и ft_42sh_pipe_right там само закроет
		close(array->pr.even_fds[PIPE_READ_42SH]);
		close(array->pr.even_fds[PIPE_WRITE_42SH]);
	}
	pid = array->pr.pid_grup;
	setpgid(0, pid);//Добавим в группу //Что бы не было состязательности//Незнаю почему но при использовании ft_42sh_pipe_left и ft_42sh_pipe_right не попадало в группу а всегда создает новую
	if (pid == 0)//Может так происходит из-за этого setpgid(0, array->pr.pid_fork); в ft_42sh_pipe_left и ft_42sh_pipe_right
		if ((jobs->b_type & AUTO_TYPE_RUN_42SH) == 0)
			ft_42sh_stub_ioctl(array, TIOCSPGRP, &array->pr.pid_fork);//сделаем группу активной//Не проверяем на коректность так как нельзя одну и туже сделать гуппу активной
}
