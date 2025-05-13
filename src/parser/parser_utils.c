#include "parser.h"
#include "error.h"

bool	parser_is_quoted_word(t_parser *parser, t_ast_node_i node)
{
	if (parser->ast.nodes[node].type == FINISHED_SINGLE_QUOTE
		|| parser->ast.nodes[node].type == FINISHED_DOUBLE_QUOTE)
		return (true);
	if (parser->ast.nodes[node].left_node)
	{
		if (parser_is_quoted_word(parser,
				parser->ast.nodes[node].left_node))
			return (true);
	}
	if (parser->ast.nodes[node].right_node)
	{
		if (parser_is_quoted_word(parser,
				parser->ast.nodes[node].right_node))
			return (true);
	}
	return (false);
}

bool	parser_tokenjoin_unquoted_wordelt(t_parser *parser, t_ast_node_i node)
{
	t_astnode	*astnode;

	astnode = &(&parser->ast)->nodes[node];
	if (astnode->type == 'w' || astnode->type == FINISHED_DOUBLE_QUOTE
		|| astnode->type == FINISHED_SINGLE_QUOTE)
	{
		if (!xbuffer_add_data(parser->token_word_buffer, astnode->str,
				ft_strlen(astnode->str)))
			return (false);
	}
	else if (astnode->type == PARSER_NODE_TYPE_WORD_ELTS)
	{
		if (!parser_tokenjoin_unquoted_wordelt(parser, astnode->left_node))
			return (false);
		if (!parser_tokenjoin_unquoted_wordelt(parser, astnode->right_node))
			return (false);
	}
	return (true);
}

bool	parser_tokenjoin_unquoted(t_parser *parser, t_ast_node_i node)
{
	size_t			size;
	char			*token_word;

	if (parser->ast.nodes[node].type != PARSER_NODE_TYPE_WORD_ELTS)
		return (true);
	parser->token_word_buffer = create_xbuffer(32);
	if (!parser->token_word_buffer)
		return (false);
	if (!parser_tokenjoin_unquoted_wordelt(parser, node))
		return (false);
	size = xbuffer_size(parser->token_word_buffer);
	token_word = xbuffer_alloc(parser->tokenizer.buffer, size + 1);
	if (!token_word)
		return (false);
	xbuffer_copy_data(parser->token_word_buffer, token_word);
	token_word[size] = 0;
	parser->ast.nodes[node].str = token_word;
	delete_xbuffer(parser->token_word_buffer);
	parser->token_word_buffer = NULL;
	return (true);
}
