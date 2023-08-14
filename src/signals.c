/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 18:57:03 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 19:27:04 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// child process ctrl+C should display on terminal ^C
// mode 1 is disable, mode 0 is enable
void	set_termios(int mode)
{
	struct termios	ttr_settings;
	int				result;

	result = tcgetattr(STDOUT_FILENO, &ttr_settings);
	if (result < 0)
	{
		internal_error_printer("Tcgetattr() error");
		g_exit_code = 1;
		exit (EXIT_FAILURE);
	}
	if (mode == 1)
		ttr_settings.c_lflag &= ~ECHOCTL;
	else
		ttr_settings.c_lflag |= ECHOCTL;
	result = tcsetattr(STDOUT_FILENO, TCSANOW, &ttr_settings);
	if (result < 0)
	{
		internal_error_printer("Sigaction() error");
		g_exit_code = 1;
		exit (EXIT_FAILURE);
	}
}

static void	handle_signals(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_redisplay();
		g_exit_code = 1;
	}
}

static void	handle_signals_child(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		g_exit_code = 128 + SIGINT;
	}
	if (signum == SIGQUIT)
	{
		ft_putendl_fd("Quit: 3", STDOUT_FILENO);
		g_exit_code = 128 + SIGQUIT;
	}
}

void	set_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &handle_signals;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("Sigaction() error");
		g_exit_code = 1;
		exit (EXIT_FAILURE);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("Sigaction() error");
		g_exit_code = 1;
		exit (EXIT_FAILURE);
	}
}

void	set_signals_child(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &handle_signals_child;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		internal_error_printer("Sigaction() error");
		g_exit_code = 1;
		exit (EXIT_FAILURE);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		internal_error_printer("Sigaction() error");
		g_exit_code = 1;
		exit (EXIT_FAILURE);
	}
}
