/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 16:05:21 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/24 20:37:26 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	unset_is_valid_identifier(char *cmd)
{
	if (!ft_isalpha(*cmd) || ft_strchr(cmd, '=') != NULL)
	{
		error_printer("unset", cmd, "not a valid identifier");
		g_exit_code = 1;
		return (0);
	}
	return (1);
}

static t_var_list	*get_previous(t_var_list *var_head, t_var_list *found_node)
{
	t_var_list	*current;
	t_var_list	*previous;

	previous = NULL;
	current = var_head;
	while (current != found_node)
	{
		previous = current;
		current = current->next;
	}
	return (previous);
}

static void	free_single_var_node(t_var_list *node)
{
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

t_var_list	*delete_first_node(t_var_list *var_list)
{
	t_var_list	*to_remove;

	to_remove = var_list;
	var_list = var_list->next;
	free_single_var_node(to_remove);
	return (var_list);
}

// int	builtin_unset(t_var_list **var_head, char **cmd)
// {
// 	int			arg_count;
// 	int			index;
// 	t_var_list	*previous_node;
// 	t_var_list	*node_found;

// 	index = 1;
// 	arg_count = get_arg_count(cmd);
// 	if (arg_count == 1)
// 		return (0);
// 	while (cmd && *(cmd + index))
// 	{
// 		if (unset_is_valid_identifier(*(cmd + index)))
// 		{
// 			node_found = get_var_list_node(*var_head, *(cmd + index));
// 			if (node_found != NULL)
// 			{
// 				previous_node = get_previous(*var_head, node_found);
// 				if (previous_node == NULL)
// 				{
//     				var_head = &(*var_head)->next;
//     				free_single_var_node(node_found);
// 				}
// 				else
// 				{
// 					previous_node->next = node_found->next;
// 					free_single_var_node(node_found);
// 				}

// 			}
// 		}
// 		index++;
// 	}
// 	return (g_exit_code);
// }

int	builtin_unset(t_minishell *ms_data, char **cmd)
{
	int			arg_count;
	int			index;
	t_var_list	*previous_node;
	t_var_list	*node_found;

	index = 1;
	arg_count = get_arg_count(cmd);
	if (arg_count == 1)
		return (0);
	while (cmd && *(cmd + index))
	{
		if (unset_is_valid_identifier(*(cmd + index)))
		{
			node_found = get_var_list_node(ms_data->var_head, *(cmd + index));
			if (node_found != NULL)
			{
				previous_node = get_previous(ms_data->var_head, node_found);
				if (previous_node == NULL)
				{
    				ms_data->var_head = ms_data->var_head->next;
    				free_single_var_node(node_found);
				}
				else
				{
					previous_node->next = node_found->next;
					free_single_var_node(node_found);
				}

			}
		}
		index++;
	}
	return (g_exit_code);
}