/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:03:31 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 18:51:42 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code;

int	get_exec_data(t_minishell *data)
{
	t_parser_exit_code	parser_ret;

	parser_ret = parser(&data->parser_data, (const char *) data->p_input);
	if (parser_ret == PARSER_SYNTAX_ERROR)
		g_exit_code = 258;
	else if (parser_ret == PARSER_MALLOC_ERROR)
		g_exit_code = 1;
	data->prev_exit = g_exit_code;
	if (parser_ret == PARSER_SUCCESS)
	{
		if (expander_executor(&data->parser_data, data->var_head) \
														== EXPANDER_SUCCESS)
			g_exit_code = 0;
		else
			g_exit_code = 1;
	}
	return (g_exit_code);
}

void	set_up_minishell(t_minishell *ms_data)
{
	set_signals();
	ms_data->var_head = NULL;
	ms_data->p_input = NULL;
	ms_data->prev_exit = 0;
	g_exit_code = 0;
}

void	minishell_loop(t_minishell *data)
{
	while (1)
	{
		set_termios(1);
		data->p_input = readline(BLUE "eunskim_tmarts minishell % " RESET);
		if (data->p_input == NULL)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			rl_clear_history();
			free_var_list(data->var_head);
			break ;
		}
		if (*data->p_input)
		{
			add_history(data->p_input);
			if (get_exec_data(data) == 0)
			{
				executor(data);
				free_ast(&data->parser_data);
			}
		}
		free(data->p_input);
	}
}

int	main(int argc, char **argv)
{
	t_minishell	data;

	if (argc != 1)
		return (0);
	(void)argv;
	set_up_minishell(&data);
	if (initiate_var_list(&data.var_head) != 0)
		exit (EXIT_FAILURE);
	minishell_loop(&data);
	return (g_exit_code);
}

// if (isatty(fileno(stdin)))
// 	data->p_input = readline(BLUE "eunskim_tmarts minishell % " RESET);
// else
// {
// 	data->p_input = get_next_line(fileno(stdin));
// 	if (data->p_input)
// 		data->p_input[ft_strlen(data->p_input) - 1] = 0;
// }