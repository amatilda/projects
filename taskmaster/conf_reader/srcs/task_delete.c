/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_delete.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wstygg <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 15:42:16 by wstygg            #+#    #+#             */
/*   Updated: 2020/08/04 17:58:11 by wstygg           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_reader.h"
#include "conf.h"

void		task_list_delete(t_list *task_list)
{
	t_list	*del;

	while (task_list)
	{
		task_delete(&task_list->task);
		del = task_list;
		task_list = task_list->next;
		free(del);
	}
}

static void	delete_help(t_task **task_p)
{
	free((*task_p)->jobs);
	(*task_p)->jobs = NULL;
	free((*task_p)->env);
	(*task_p)->env = NULL;
	free(*task_p);
	*task_p = NULL;
}

void		task_delete(t_task **task_p)
{
	int		i;

	if (!task_p || !*task_p)
		return ;
	i = -1;
	free((*task_p)->name);
	((*task_p)->name) = NULL;
	free((*task_p)->command);
	(*task_p)->command = NULL;
	free((*task_p)->directory);
	(*task_p)->directory = NULL;
	if ((*task_p)->tstderr != NULL)
		free((*task_p)->tstderr);
	if ((*task_p)->tstdout != NULL)
		free((*task_p)->tstdout);
	(*task_p)->tstderr = NULL;
	(*task_p)->tstdout = NULL;
	if ((*task_p)->env)
		while ((*task_p)->env[++i])
		{
			free((*task_p)->env[i]);
			(*task_p)->env[i] = NULL;
		}
	delete_help(task_p);
}
