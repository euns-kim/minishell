/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignments_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 21:07:31 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/27 21:55:25 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	ft_sub_len(char const *start, char c)
{
	int	len;

	len = 0;
	while (*(start + len) != 0 && *(start + len) != c)
		len++;
	return (len);
}

t_var_list	*get_var_list_node(t_var_list *var_head, char *str)
{
	t_var_list	*current;
	int			newkey_len;

	newkey_len = ft_sub_len(str, '=');
	current = var_head;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, str, newkey_len) == 0 \
			&& (int)ft_strlen(current->key) == newkey_len)
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	modify_var_list_node(t_var_list *matching_node, char *cmd)
{
	char		*delimiter;
	char		*new_value;

	delimiter = ft_strchr(cmd, '=');
	if (delimiter != NULL)
	{
		new_value = ft_strdup(delimiter + 1);
		if (!new_value)
			return (internal_error_printer("Malloc failed"), EXIT_FAILURE);
		free(matching_node->value);
		matching_node->value = new_value;
	}
	return (EXIT_SUCCESS);
}

static int	full_assign(t_var_list *new, char *word, char *delim_pt)
{
	new->key = ft_strdup_pt(word, delim_pt);
	if (!new->key)
	{
		free(new);
		return (internal_error_printer("Malloc failed"), EXIT_FAILURE);
	}
	new->value = ft_strdup(delim_pt + 1);
	if (!new->value)
	{
		free(new->key);
		free(new);
		return (internal_error_printer("Malloc failed"), EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	add_to_var_list(t_var_list *var_list, char *str, int flag)
{
	t_var_list		*new_node;
	char			*delim_pt;

	new_node = (t_var_list *)ft_calloc(1, sizeof(t_var_list));
	if (!new_node)
		return (internal_error_printer("Malloc failed"), EXIT_FAILURE);
	new_node->env_flag = flag;
	delim_pt = ft_strchr(str, '=');
	if (delim_pt == NULL)
	{
		new_node->key = ft_strdup(str);
		if (!new_node->key)
		{
			free(new_node);
			return (internal_error_printer("Malloc failed"), EXIT_FAILURE);
		}
	}
	else if (full_assign(new_node, str, delim_pt) != 0)
		return (EXIT_FAILURE);
	ft_lstadd_back_ms(&var_list, new_node);
	return (EXEC_SUCCESS);
}
