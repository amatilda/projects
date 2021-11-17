/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_exe_fun.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 21:58:20 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:21:12 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_42sh_exe.h"

static size_t	fn_test_fork(register t_jobs_42sh *jobs,
void (*f)(register t_main_42sh *array, register char **lp_arg))
{
	register t_jobs_42sh		*tmp;

	if ((jobs->b_type & AUTO_TYPE_RUN_42SH) != 0)
		return (1);
	if (f == ft_42sh_exe_set || jobs->fd_pipe == 0)//Нету пайпа тоже не запускаем в отдельном или есть пайп но устанавливаем локальные переменные
		return (0);
	if (f == ft_42sh_cm_exit)//Так же запускаем в форке если выход по для чеклиста специально
		return (1);
	if ((tmp = jobs->next) == 0 || tmp->count == 1)//Если последний то не запускаем в отдельном процессе хоть пайп
		return (0);
	return (1);
}

static size_t	fn_error(register t_main_42sh *array,
register t_jobs_42sh *jobs, int *fds)
{
	close(fds[PIPE_READ_42SH]);
	close(fds[PIPE_WRITE_42SH]);
	return (ft_42sh_exe_err_foor(array, jobs));
}

static size_t	fn_finish(register t_main_42sh *array,
register t_jobs_42sh *jobs, register void *pwd,
void (*f)(register t_main_42sh *array, register char **lp_arg))
{
	register t_jobs_42sh		*tmp;

	if (f == ft_42sh_cm_cd && jobs->fd_pipe != 0 && pwd != array->pwd.path.buff
	&& ((tmp = jobs->next) == 0 || tmp->count == 1))//Если "cd" и последний с пайпом  то сообщим что все равно изменили дерикторию
	{
		ft_write_buffer_str_zero(&array->out, "(pwd now: ");
		ft_write_buffer_str_zero(&array->out, (void *)array->pwd.path_view);
		ft_write_buffer_str_zero(&array->out, ")\n");
		ft_write_buffer(&array->out);
	}
	if (jobs->stat_loc == STATUS_JOBS_STOP_42SH)//Значит прекращаем дальнейший выполнение команд
		return (0);
	return (1);
}

static size_t	fn_not_fork(register t_main_42sh *array,
register t_jobs_42sh *jobs, char **lp_arg,
void (*f)(register t_main_42sh *array, register char **lp_arg))
{
	register pid_t				pid;
	int							fds[2];
	register void				*pwd;

	if (f != ft_42sh_exe_err && (jobs->b_fd_left != 0 || jobs->b_fd_right != 0))//Вынужденная мера что из-за терминала множественное перенаправление не падало в суспенд
	{//В случае вывода сообщения об ошибки редирект не нужен
		if (pipe(&fds[0]) != 0)
			ft_42sh_exit(E_PIPE_CODE_42SH, __FILE__, __func__, __LINE__);
		if ((pid = fork()) == 0)
		{
			ft_42sh_exe_fork(array, jobs, &fds[0]);//Так как мы долго тут не будем и пайп тем самым ну никак не переполнимм то в булт ин не будем закрывать не нужные fd по правильному надо бы
			ft_42sh_cm_exit_fun(array, array->env.exit_status->number);
		}
		else if (pid < 0)//Если ошибка то прекрашаем но сначала закроем лишний пайп
			return (fn_error(array, jobs, &fds[0]));
		array->pr.pid_not_fork = pid;
		ft_42sh_exe_grup(array, jobs, pid, &fds[0]);
	}
	pwd = array->pwd.path.buff;
	ft_42sh_exe_fun_run(array, jobs, lp_arg, f);
	return (fn_finish(array, jobs, pwd, f));
}

size_t			ft_42sh_exe_fun(register t_main_42sh *array,
register t_jobs_42sh *jobs, char **lp_arg,
void (*f)(register t_main_42sh *array, register char **lp_arg))//fd ->write | read<-fd fd2->write | read<-fd2 fd->write | read<-fd
{
	register pid_t				pid;
	int							fds[2];

	if (fn_test_fork(jobs, f) != 0)//((tmp = jobs->next) == 0 || tmp->count == 1)//Потом сделать что бы последний был не в процессе но тут проплемма с приотановкай//!!!
	{
		if (pipe(&fds[0]) != 0)
			ft_42sh_exit(E_PIPE_CODE_42SH, __FILE__, __func__, __LINE__);
		if ((pid = fork()) == 0)
		{
			ft_42sh_exe_fork(array, jobs, &fds[0]);//Так как мы долго тут не будем и пайп тем самым ну никак не переполнимм то в булт ин не будем закрывать не нужные fd по правильному надо бы
			ft_42sh_exe_fun_run(array, jobs, lp_arg, f);
			ft_42sh_cm_exit_fun(array, array->env.exit_status->number);
		}
		else if (pid < 0)//Если ошибка то прекрашаем но сначала закроем лишний пайп
			return (fn_error(array, jobs, &fds[0]));
		ft_42sh_exe_grup(array, jobs, pid, &fds[0]);
		return (1);
	}
	return (fn_not_fork(array, jobs, lp_arg, f));
}
