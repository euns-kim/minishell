/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eunskim <eunskim@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:10:48 by eunskim           #+#    #+#             */
/*   Updated: 2023/07/28 16:29:51 by eunskim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	cd_to_oldpwd(t_var_list *var_head, char *pwd)
{
	char	*oldpwd;

	oldpwd = get_value_for_key(var_head, "OLDPWD");
	if (oldpwd == NULL)
		return (free(pwd), \
		error_printer("cd", NULL, "OLDPWD not set"), EXIT_FAILURE);
	if (oldpwd[0] == '\0')
		return (free(pwd), EXIT_SUCCESS);
	if (chdir(oldpwd) == -1)
		return (free(pwd), \
		error_printer("cd", NULL, strerror(errno)), EXIT_FAILURE);
	oldpwd = ft_strdup(oldpwd);
	if (oldpwd == NULL)
		return (free(pwd), \
		internal_error_printer("Malloc failed"), EXIT_FAILURE);
	if (change_value_for_key(var_head, "OLDPWD", pwd))
		return (free(pwd), free(oldpwd), \
		internal_error_printer("Malloc failed"), EXIT_FAILURE);
	if (change_value_for_key(var_head, "PWD", oldpwd))
		return (free(pwd), free(oldpwd), \
		internal_error_printer("Malloc failed"), EXIT_FAILURE);
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	return (free(pwd), free(oldpwd), EXIT_SUCCESS);
}

int	cd_to_home(t_var_list *var_head, char *pwd)
{
	char	*home;

	home = get_value_for_key(var_head, "HOME");
	if (home == NULL)
		return (free(pwd), \
		error_printer("cd", NULL, "HOME not set"), EXIT_FAILURE);
	if (home[0] == '\0')
		return (free(pwd), EXIT_SUCCESS);
	if (chdir(home) == -1)
	{
		free(pwd);
		if (errno == ENOENT)
			return (error_printer("cd", home, strerror(errno)), EXIT_FAILURE);
		else
			return (error_printer("cd", NULL, strerror(errno)), EXIT_FAILURE);
	}
	if (change_value_for_key(var_head, "OLDPWD", pwd))
		return (free(pwd), \
		internal_error_printer("Malloc failed"), EXIT_FAILURE);
	if (change_value_for_key(var_head, "PWD", home))
		return (free(pwd), \
		internal_error_printer("Malloc failed"), EXIT_FAILURE);
	return (free(pwd), EXIT_SUCCESS);
}

int	cd_with_path(t_var_list *var_head, char **cmd, char *pwd)
{
	if (chdir(cmd[1]) == -1)
	{
		free(pwd);
		if (errno == ENOENT)
			return (error_printer("cd", cmd[1], strerror(errno)), EXIT_FAILURE);
		else
			return (error_printer("cd", NULL, strerror(errno)), EXIT_FAILURE);
	}
	if (change_value_for_key(var_head, "OLDPWD", pwd))
		return (free(pwd), \
		internal_error_printer("Malloc failed"), EXIT_FAILURE);
	free(pwd);
	pwd = getcwd(NULL, PATH_MAX);
	if (pwd == NULL)
		return (error_printer("cd", NULL, strerror(errno)), EXIT_FAILURE);
	if (change_value_for_key(var_head, "PWD", pwd))
		return (free(pwd), \
		internal_error_printer("Malloc failed"), EXIT_FAILURE);
	return (free(pwd), EXIT_SUCCESS);
}

int	execute_cd(t_var_list *var_head, char **cmd)
{
	char	*pwd;

	pwd = getcwd(NULL, PATH_MAX);
	if (pwd == NULL)
		return (error_printer("cd", NULL, strerror(errno)), EXIT_FAILURE);
	if (cmd[1] == NULL || (cmd[1][0] == '~' && cmd[1][1] == '\0'))
		return (cd_to_home(var_head, pwd));
	else if (cmd[1][0] == '-' && cmd[1][1] == '\0')
		return (cd_to_oldpwd(var_head, pwd));
	else if (cmd[1][0] == '\0')
		return (free(pwd), EXIT_SUCCESS);
	else
		return (cd_with_path(var_head, cmd, pwd));
}

int	builtin_cd(t_var_list *var_head, char **cmd)
{
	if (check_if_in_var_list(var_head, "PWD") != true)
	{
		if (add_to_var_list(var_head, "PWD", 0) != EXEC_SUCCESS)
			return (EXIT_FAILURE);
	}
	if (check_if_in_var_list(var_head, "OLDPWD") != true)
	{
		if (add_to_var_list(var_head, "OLDPWD", 0) != EXEC_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (execute_cd(var_head, cmd));
}
