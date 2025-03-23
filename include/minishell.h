/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 16:09:17 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 18:23:19 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <stdbool.h>
# include <fcntl.h> //defines O_RDONLY etc
# include "libft.h"
# include "parser.h"
# include "expander.h"
# include "execution.h"
# include "builtin.h"

# define BLUE	"\033[34;1m\002"
# define RESET	"\001\e[0m\002"

extern int					g_exit_code;
extern char					**environ;

typedef struct s_var_list	t_var_list;

typedef struct s_var_list
{
	char		*key;
	char		*value;
	bool		env_flag;
	t_var_list	*next;
}	t_var_list;

typedef struct s_minishell
{
	t_var_list	*var_head;
	t_parser	parser_data;
	char		*p_input;
	int			prev_exit;
}	t_minishell;

int		initiate_var_list(t_var_list **var_list);
void	free_var_list(t_var_list *var_list);
void	ft_lstadd_back_ms(t_var_list **var_list, t_var_list *new);

void	set_signals(void);
void	set_signals_child(void);
void	set_termios(int mode);

#endif