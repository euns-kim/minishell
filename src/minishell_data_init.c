/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_data_init.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:19:17 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 19:26:01 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_back_ms(t_var_list **var_list, t_var_list *new)
{
	t_var_list	*last;

	if (*var_list == 0)
	{
		*var_list = new;
		return ;
	}		
	last = *var_list;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
	return ;
}

static int	add_oldpwd_node(t_var_list **var_list)
{
	t_var_list	*oldpwd;

	oldpwd = (t_var_list *)ft_calloc(1, sizeof(t_var_list));
	if (!oldpwd)
		return (free_var_list(*var_list), 1);
	oldpwd->env_flag = 1;
	oldpwd->key = ft_strdup("OLDPWD");
	if (!oldpwd->key)
		return (free_var_list(*var_list), 1);
	oldpwd->value = NULL;
	ft_lstadd_back_ms(var_list, oldpwd);
	return (0);
}

static t_var_list	*make_new_node(char *environ_var)
{
	t_var_list	*new_var_node;
	char		*delimiter_pt;

	new_var_node = (t_var_list *)ft_calloc(1, sizeof(t_var_list));
	if (!new_var_node)
		return (NULL);
	new_var_node->env_flag = 1;
	delimiter_pt = ft_strchr(environ_var, '=');
	if (delimiter_pt == NULL)
	{
		new_var_node->key = ft_strdup(environ_var);
		if (!new_var_node->key)
			return (free(new_var_node), NULL);
		new_var_node->value = NULL;
	}
	else
	{
		new_var_node->key = ft_strdup_pt(environ_var, delimiter_pt);
		if (!new_var_node->key)
			return (free(new_var_node), NULL);
		new_var_node->value = ft_strdup(delimiter_pt + 1);
		if (!new_var_node->value)
			return (free(new_var_node->key), free(new_var_node), NULL);
	}
	return (new_var_node);
}

int	initiate_var_list(t_var_list **var_list)
{
	t_var_list	*new_var_node;
	int			i;

	i = 0;
	while (environ[i] != NULL)
	{
		if (ft_strncmp(environ[i], "OLDPWD", 6) != 0)
		{
			new_var_node = make_new_node(environ[i]);
			if (!new_var_node)
				return (free_var_list(*var_list), \
					internal_error_printer("Malloc failed"), 1);
			ft_lstadd_back_ms(var_list, new_var_node);
		}
		i++;
	}
	return (add_oldpwd_node(var_list));
}

void	free_var_list(t_var_list *var_list)
{
	t_var_list	*free_this;

	while (var_list != NULL)
	{
		if (var_list->key)
			free (var_list->key);
		if (var_list->value)
			free (var_list->value);
		free_this = var_list;
		var_list = var_list->next;
		free (free_this);
	}
}
