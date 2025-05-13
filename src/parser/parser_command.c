#include "parser.h"
#include "error.h"

char	*parser_test_opt_command_elements(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;
	t_ast_node_i	left_node;

	left_node = parser->stored_node;
	result_str = parser_test_command_element(parser, in_str);
	if (parser->parser_error)
		return (NULL);
	if (!result_str)
		return (in_str);
	new_node = ast_new_node(&parser->ast, ',', NULL);
	if (!new_node)
		return (parser_set_error(parser, MEMORY_ALLOC));
	parser->ast.nodes[new_node].left_node = left_node;
	result_str = parser_test_opt_command_elements(parser, result_str);
	if (!result_str)
		return (NULL);
	parser->ast.nodes[new_node].right_node = parser->stored_node;
	parser->stored_node = new_node;
	return (result_str);
}

char	*parser_test_command_element(t_parser *parser, char *in_str)
{
	char	*result_str;

	result_str = parser_test_word(parser, in_str);
	if (result_str)
		return (result_str);
	result_str = parser_test_redir(parser, in_str);
	return (result_str);
}

char	*parser_test_command2(t_parser *parser, char *in_str)
{
	char	*result_str;

	if (parser->tokenizer.token_type == ')')
	{
		in_str = parser_accept(parser, parser->tokenizer.token_type);
		if (!in_str)
			return (NULL);
		parser->stored_node = 0;
		result_str = parser_test_redir(parser, in_str);
		if (parser->parser_error)
			return (NULL);
		if (!result_str)
			return (in_str);
		return (parser_test_opt_redirs(parser, result_str));
	}
	if (!parser->tokenizer.token_type)
		return (parser_set_error(parser, MISSING_PARENTHESIS));
	return (NULL);
}

char	*parser_test_command(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;

	if (parser->tokenizer.token_type == '(')
		return (parser_test_subshell(parser, in_str));
	result_str = parser_test_command_element(parser, in_str);
	if (!result_str)
		return (NULL);
	new_node = ast_new_node(&parser->ast, 'x', NULL);
	if (!new_node)
		return (parser_set_error(parser, MEMORY_ALLOC));
	result_str = parser_test_opt_command_elements(parser, result_str);
	parser->ast.nodes[new_node].left_node = parser->stored_node;
	parser->stored_node = new_node;
	return (parser->stored_node = new_node, result_str);
}
