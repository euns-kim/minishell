/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piper.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 14:41:50 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 18:36:02 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	make_pipes(int *pipe1, int *pipe2, int child_nr)
{
	if (child_nr % 2 == 0)
	{
		if (pipe(pipe2) == -1)
			return (internal_error_printer("Pipe error"), 1);
	}
	else
	{
		if (pipe(pipe1) == -1)
			return (internal_error_printer("Pipe error"), 1);
	}	
	return (0);
}

void	close_used_pipes_and_fds(t_piper *piper_data)
{
	if (piper_data->child_nr != 1 \
		&& piper_data->child_nr <= piper_data->fork_count)
	{
		if (piper_data->child_nr % 2 == 0)
		{
			close(piper_data->pipe1[0]);
			close(piper_data->pipe1[1]);
		}
		else
		{
			close(piper_data->pipe2[0]);
			close(piper_data->pipe2[1]);
		}
	}
	if (piper_data->cmd_node->stdin_redirect != NULL \
		&& piper_data->fd_in_out[0] <! 0)
		close(piper_data->fd_in_out[0]);
	if (piper_data->cmd_node->stdout_redirect != NULL \
		&& piper_data->fd_in_out[1] <! 0)
		close(piper_data->fd_in_out[1]);
}

void	ft_waiting(int *pids, int nr_of_forks)
{
	t_wait	s_wait;
	int		pid_index;

	pid_index = 0;
	while (pid_index < nr_of_forks)
	{
		if (pid_index == nr_of_forks - 1)
			waitpid(pids[pid_index], &s_wait.wstatus, 0);
		else
			waitpid(pids[pid_index], NULL, 0);
		pid_index++;
	}
	if (WIFEXITED(s_wait.wstatus))
	{
		s_wait.status_code = 0;
		s_wait.status_code = WEXITSTATUS(s_wait.wstatus);
		g_exit_code = s_wait.status_code;
	}
}

t_exec_exit_code	piper(t_minishell *ms_data, t_parser *parser_data)
{
	int		i;
	t_piper	piper;

	i = -1;
	set_signals_child();
	if (init_piper_data(parser_data, &piper) != 0)
		return (internal_error_printer("Malloc failed"), EXEC_MALLOC_ERROR);
	while (++i <= piper.fork_count - 1)
	{
		if (i < piper.fork_count - 1 && \
		(make_pipes(piper.pipe1, piper.pipe2, (i + 1)) != 0))
			return (free(piper.pids), PIPE_ERROR);
		update_child(parser_data, &piper, i + 1);
		piper.pids[i] = fork();
		if (piper.pids[i] == -1)
			return (free(piper.pids), internal_error_printer("Fork error"), 4);
		if (piper.pids[i] == 0)
			child_with_pipes(ms_data, &piper);
		close_used_pipes_and_fds(&piper);
	}
	ft_waiting(piper.pids, piper.fork_count);
	set_signals();
	if (g_exit_code != 0)
		return (free(piper.pids), EXEC_FAIL);
	return (free(piper.pids), EXEC_SUCCESS);
}
