/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 14:34:18 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 18:22:07 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_on_exit(t_minishell *ms_data)
{
	if (ms_data->p_input)
		free(ms_data->p_input);
	if (ms_data->var_head)
		free_var_list(ms_data->var_head);
	free_ast(&ms_data->parser_data);
	rl_clear_history();
}

int	builtin_exit(t_minishell *ms_data, char **cmd)
{
	long long int	exit_local;
	int				arg_count;

	ft_putendl_fd("exit", 1);
	arg_count = get_arg_count(cmd);
	if (arg_count == 1)
		exit_local = ms_data->prev_exit;
	if (arg_count > 1)
	{
		if (cmd[1][0] == '\0' || ft_atoi_secure(cmd[1], &exit_local) != 0)
		{
			error_printer("exit", cmd[1], "numeric argument required");
			exit_local = 255;
		}
	}
	if (arg_count > 2 && exit_local != 255)
	{
		error_printer("exit", NULL, "too many arguments");
		return (1);
	}
	if (exit_local < 0 || exit_local > 255)
		exit_local = exit_local % 256;
	free_on_exit(ms_data);
	g_exit_code = exit_local;
	exit (g_exit_code);
}
