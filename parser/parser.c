/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 17:48:02 by eunskim           #+#    #+#             */
/*   Updated: 2023/07/22 21:32:35 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parser_exit_code	parse_pipe(t_parser *data, t_token_scanner *scanner)
{
	t_ast	*cmd_node;

	if (data->malloc_failed == true)
		return (PARSER_FAILURE);
	if (peek_token(scanner) == TOKEN_PIPE)
	{
		cmd_node = (t_ast *) ft_calloc(1, sizeof(t_ast));
		if (cmd_node == NULL)
		{
			data->malloc_failed = true;
			return (PARSER_FAILURE);
		}
		add_new_ast_node_pipe(data, cmd_node);
		advance_token_list(scanner);
		return (PARSER_SUCCESS);
	}
	else
		return (PARSER_FAILURE);
}

static t_parser_exit_code	parse_command_logic(t_parser *data, \
t_token_scanner *scanner, t_ast *cmd_node)
{
	t_parser_exit_code	ret;

	ret = parse_cmd_prefix(data, scanner, cmd_node);
	if (ret == PARSER_FAILURE)
	{
		ret = parse_cmd_word(data, scanner, cmd_node);
		if (ret == PARSER_FAILURE)
			return (ret);
	}
	else
	{
		if (parse_cmd_word(data, scanner, cmd_node) == PARSER_FAILURE)
			return (ret);
	}
	parse_cmd_suffix(data, scanner, cmd_node);
	return (ret);
}

t_parser_exit_code	parse_command(t_parser *data, t_token_scanner *scanner)
{
	t_ast				*cmd_node;

	cmd_node = (t_ast *) ft_calloc(1, sizeof(t_ast));
	if (cmd_node == NULL)
	{
		data->malloc_failed = true;
		return (PARSER_FAILURE);
	}
	cmd_node->content = (t_ast_content *) ft_calloc(1, sizeof(t_ast_content));
	if (cmd_node->content == NULL)
	{
		free(cmd_node);
		data->malloc_failed = true;
		return (PARSER_FAILURE);
	}
	add_new_ast_node_cmd(data, cmd_node);
	return (parse_command_logic(data, scanner, cmd_node));
}

t_parser_exit_code	parse_complete_command(t_parser *data, \
t_token_scanner *scanner)
{
	if (parse_command(data, scanner) == PARSER_FAILURE)
		return (PARSER_FAILURE);
	if (token_list_is_at_end(scanner) == true)
		return (PARSER_SUCCESS);
	if (parse_pipe(data, scanner) == PARSER_FAILURE)
		return (PARSER_FAILURE);
	if (parse_complete_command(data, scanner) == PARSER_FAILURE)
		return (PARSER_FAILURE);
	return (PARSER_SUCCESS);
}

t_parser_exit_code	parser(t_parser *data, const char *source)
{
	t_lexer_exit_code	lexer_ret;
	t_lexer				lexer_data;
	t_token_scanner		scanner;
	t_parser_exit_code	parser_ret;

	lexer_ret = lexer(&lexer_data, source);
	if (lexer_ret != LEXER_SUCCESS)
		return ((t_parser_exit_code)((int) lexer_ret + 1));
	init_token_scanner(&scanner, lexer_data.head);
	init_parser_data(data, &scanner);
	if (lexer_data.head->token.type == TOKEN_EOF)
		return (free_token_list(&lexer_data), PARSER_SUCCESS);
	parser_ret = parse_complete_command(data, &scanner);
	if (data->malloc_failed == true)
	{
		lexer_error_printer("internal error", "Malloc failed");
		return (parser_free(data, &lexer_data), PARSER_MALLOC_ERROR);
	}
	if (parser_ret == PARSER_FAILURE)
	{
		syntax_error_printer(&scanner);
		return (parser_free(data, &lexer_data), PARSER_SYNTAX_ERROR);
	}
	return (free_token_list(&lexer_data), PARSER_SUCCESS);
}
