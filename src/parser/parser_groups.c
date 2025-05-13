#include "parser.h"
#include "error.h"

char	*parser_test_commands(t_parser *parser, char *in_str)
{
	char	*result_str;

	result_str = parser_test_pipeline(parser, in_str);
	if (!result_str)
		return (NULL);
	result_str = parser_test_opt_conditionnal_chain(parser, result_str);
	if (!result_str)
		return (NULL);
	return (result_str);
}

char	*parser_test_subshell(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;

	(void) in_str;
	if (parser->tokenizer.token_type != '(')
		return (NULL);
	new_node = ast_new_node(&parser->ast, '(', NULL);
	if (!new_node)
		return (parser_set_error(parser, MEMORY_ALLOC));
	result_str = parser_accept(parser, parser->tokenizer.token_type);
	if (!result_str)
		return (NULL);
	if (!parser->tokenizer.token_type)
		return (parser->parser_error = MISSING_COMMAND, NULL);
	result_str = parser_test_commands(parser, result_str);
	if (!result_str)
		return (NULL);
	parser->ast.nodes[new_node].left_node = parser->stored_node;
	result_str = parser_test_command2(parser, result_str);
	if (!result_str)
		return (NULL);
	parser->ast.nodes[new_node].right_node = parser->stored_node;
	return (parser->stored_node = new_node, result_str);
}
