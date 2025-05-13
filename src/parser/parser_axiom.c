#include "parser.h"
#include "error.h"

char	*parser_test_opt_conditionnal_chain(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	nwnode;

	if (parser->tokenizer.token_type != COND_AND
		&& parser->tokenizer.token_type != COND_OR)
		return (in_str);
	nwnode = ast_new_node(&parser->ast, parser->tokenizer.token_type, NULL);
	if (!nwnode)
		return (parser_set_error(parser, MEMORY_ALLOC));
	result_str = parser_accept(parser, parser->tokenizer.token_type);
	if (!result_str)
		return (NULL);
	if (!parser->tokenizer.token_type)
		return (parser->parser_error = MISSING_COMMAND, NULL);
	parser->ast.nodes[nwnode].left_node = parser->stored_node;
	result_str = parser_test_pipeline(parser, result_str);
	if (!result_str)
		return (NULL);
	parser->ast.nodes[nwnode].right_node = parser->stored_node;
	parser->stored_node = nwnode;
	result_str = parser_test_opt_conditionnal_chain(parser, result_str);
	if (!result_str)
		return (NULL);
	return (result_str);
}

char	*parser_test_opt_pipe_chain(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;

	if (parser->tokenizer.token_type == '|')
	{
		result_str = parser_accept(parser, parser->tokenizer.token_type);
		if (!result_str)
			return (NULL);
		new_node = ast_new_node(&parser->ast, '|', NULL);
		if (!new_node)
			return (parser_set_error(parser, MEMORY_ALLOC));
		if (!parser->tokenizer.token_type)
			return (parser_set_error(parser, MISSING_COMMAND));
		parser->ast.nodes[new_node].left_node = parser->stored_node;
		result_str = parser_test_command(parser, result_str);
		if (!result_str)
			return (NULL);
		result_str = parser_test_opt_pipe_chain(parser, result_str);
		if (!result_str)
			return (NULL);
		parser->ast.nodes[new_node].right_node = parser->stored_node;
		return (parser->stored_node = new_node, result_str);
	}
	return (in_str);
}

char	*parser_test_pipeline(t_parser *parser, char *in_str)
{
	char	*result_str;

	result_str = parser_test_command(parser, in_str);
	if (!result_str)
		return (NULL);
	result_str = parser_test_opt_pipe_chain(parser, result_str);
	if (!result_str)
		return (NULL);
	return (result_str);
}

char	*parser_axiom_line(t_parser *parser, char *in_str)
{
	char	*result_str;

	result_str = parser_test_commands(parser, in_str);
	if (!result_str)
		return (NULL);
	parser->ast.nodes[0].left_node = parser->stored_node;
	return (parser_accept(parser, EOL));
}
