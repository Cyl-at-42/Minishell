#include "parser.h"
#include "error.h"

char	*parser_test_opt_word_elements(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;
	t_ast_node_i	left_node;

	left_node = parser->stored_node;
	result_str = parser_test_word_element(parser, in_str);
	if (parser->parser_error)
		return (NULL);
	if (!result_str)
		return (in_str);
	new_node = ast_new_node(&parser->ast, PARSER_NODE_TYPE_WORD_ELTS, NULL);
	if (!new_node)
		return (parser_set_error(parser, MEMORY_ALLOC));
	parser->ast.nodes[new_node].left_node = left_node;
	if (parser->tokenizer.token_is_spaced)
	{
		parser->ast.nodes[new_node].right_node = parser->stored_node;
		return (parser->stored_node = new_node, result_str);
	}
	result_str = parser_test_opt_word_elements(parser, result_str);
	if (!result_str)
		return (NULL);
	parser->ast.nodes[new_node].right_node = parser->stored_node;
	return (parser->stored_node = new_node, result_str);
}

char	*parser_test_word_element(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;

	(void)in_str;
	if (parser->tokenizer.token_type == 'w'
		|| parser->tokenizer.token_type == FINISHED_SINGLE_QUOTE
		|| parser->tokenizer.token_type == FINISHED_DOUBLE_QUOTE)
	{
		new_node = ast_new_node(&parser->ast, parser->tokenizer.token_type,
				parser->tokenizer.token_content);
		if (!new_node)
			return (parser_set_error(parser, MEMORY_ALLOC));
		parser->stored_node = new_node;
		result_str = parser_accept(parser, parser->tokenizer.token_type);
		if (!result_str)
			return (NULL);
		return (result_str);
	}
	return (NULL);
}

char	*parser_test_word(t_parser *parser, char *in_str)
{
	char	*result_str;

	result_str = parser_test_word_element(parser, in_str);
	if (!result_str)
		return (NULL);
	if (parser->tokenizer.token_is_spaced)
		return (result_str);
	result_str = parser_test_opt_word_elements(parser, result_str);
	if (!result_str)
		return (NULL);
	return (result_str);
}
