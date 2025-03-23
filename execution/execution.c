/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 16:11:11 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 18:46:02 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <sys/stat.h>

static t_exec_exit_code	one_fork(t_minishell *ms_data, char **cmd)
{
	int				pid;

	set_signals_child();
	pid = fork();
	if (pid == -1)
		return (internal_error_printer("Fork error"), FORK_ERROR);
	if (pid == 0)
	{
		set_termios(0);
		child_execve_process(ms_data, cmd);
	}
	ft_waiting(&pid, 1);
	set_signals();
	if (g_exit_code != 0)
		return (EXEC_FAIL);
	return (EXEC_SUCCESS);
}

static t_exec_exit_code	single_cmd(t_minishell *ms_data, \
										t_ast_content *cmd_node, int *fildes)
{
	int	save_stdin_out[2];

	save_stdin_out[0] = dup(STDIN_FILENO);
	save_stdin_out[1] = dup(STDOUT_FILENO);
	redirect(fildes[0], fildes[1]);
	if (is_builtin(cmd_node->cmd[0]))
	{
		if (ft_strncmp(cmd_node->cmd[0], "exit", 5) == 0)
		{
			restore_redirect(save_stdin_out[0], save_stdin_out[1]);
			g_exit_code = builtin_exit(ms_data, cmd_node->cmd);
		}
		else
			g_exit_code = run_builtin(ms_data, cmd_node->cmd);
	}
	else
	{
		if (one_fork(ms_data, cmd_node->cmd) != EXEC_SUCCESS)
			return (restore_redirect(save_stdin_out[0], save_stdin_out[1]), \
																	EXEC_FAIL);
	}
	restore_redirect(save_stdin_out[0], save_stdin_out[1]);
	return (EXEC_SUCCESS);
}

static t_exec_exit_code	single_node(t_minishell *ms_data, \
														t_ast_content *cmd_node)
{
	int	fd_in_out[2];

	fd_in_out[0] = STDIN_FILENO;
	fd_in_out[1] = STDOUT_FILENO;
	if (cmd_node->cmd == NULL && cmd_node->assignments != NULL)
	{
		if (add_assignments(ms_data->var_head, \
									cmd_node->assignments) != EXEC_SUCCESS)
			return (EXEC_FAIL);
	}
	if (open_files(fd_in_out, cmd_node->stdin_redirect, \
												cmd_node->stdout_redirect) != 0)
		return (EXEC_FAIL);
	if (cmd_node->cmd != NULL && single_cmd(ms_data, cmd_node, fd_in_out) \
															!= EXEC_SUCCESS)
		return (EXEC_FAIL);
	return (EXEC_SUCCESS);
}

t_exec_exit_code	executor(t_minishell *ms_data)
{	
	if (ms_data->parser_data.ast_root != NULL)
	{
		if (ms_data->parser_data.ast_root->content != NULL)
			return \
			(single_node(ms_data, ms_data->parser_data.ast_root->content));
		else
			return (piper(ms_data, &ms_data->parser_data));
	}
	return (EXEC_SUCCESS);
}
