#include "parser.h"
#include "error.h"

char	*parser_test_opt_redirs(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;
	t_ast_node_i	left_node;

	left_node = parser->stored_node;
	result_str = parser_test_redir(parser, in_str);
	if (parser->parser_error)
		return (NULL);
	if (!result_str)
		return (in_str);
	new_node = ast_new_node(&parser->ast, ',', NULL);
	if (!new_node)
		return (parser_set_error(parser, MEMORY_ALLOC));
	parser->ast.nodes[new_node].left_node = left_node;
	result_str = parser_test_opt_redirs(parser, result_str);
	parser->ast.nodes[new_node].right_node = parser->stored_node;
	parser->stored_node = new_node;
	return (result_str);
}

char	*parser_test_redir_inout(t_parser *parser, char *in_str,
	t_ast_node_i	new_node)
{
	char	*result_str;
	char	*tokenword_start;
	char	*tokenword_str;
	size_t	size;

	tokenword_start = in_str;
	result_str = parser_test_word(parser, in_str);
	if (!result_str)
		return (parser_set_error(parser, GENERIC_SYNTAX));
	size = result_str - tokenword_start;
	tokenword_str = xbuffer_alloc(parser->tokenizer.buffer, size + 1);
	if (!tokenword_str)
		return (parser_set_error(parser, MEMORY_ALLOC));
	while (tokenword_start[size - 1] == 32
		|| tokenword_start[size - 1] == '\t')
		size--;
	ft_memcpy(tokenword_str, tokenword_start, size);
	tokenword_str[size] = 0;
	parser->ast.nodes[new_node].str = tokenword_str;
	return (result_str);
}

char	*parser_test_redir_heredoc(t_parser *parser, char *in_str,
	t_ast_node_i	new_node)
{
	t_xstring		exp_string;
	char			*result_str;
	char			*heredoc;
	int				hd_result;

	result_str = parser_test_word(parser, in_str);
	if (!result_str)
		return (parser_set_error(parser, GENERIC_SYNTAX));
	if (!parser_tokenjoin_unquoted(parser, parser->stored_node))
		return (parser_set_error(parser, MEMORY_ALLOC));
	heredoc = xbuffer_alloc(parser->tokenizer.buffer, 73);
	if (!heredoc)
		return (parser_set_error(parser, MEMORY_ALLOC));
	if (parser_is_quoted_word(parser, parser->stored_node))
		hd_result = parser->get_heredoc(parser->app_info, heredoc,
				parser->ast.nodes[parser->stored_node].str, NULL);
	else
		hd_result = parser->get_heredoc(parser->app_info, heredoc,
				parser->ast.nodes[parser->stored_node].str, &exp_string);
	parser->ast.nodes[new_node].str = NULL;
	if (hd_result == -1)
		return (parser_set_error(parser, HEREDOC));
	if (hd_result == 1 || hd_result == -2)
		return (parser_set_error(parser, HEREDOC_CLEAN));
	return (parser->ast.nodes[new_node].str = heredoc, result_str);
}

char	*parser_test_redir(t_parser *parser, char *in_str)
{
	char			*result_str;
	t_ast_node_i	new_node;
	t_tokentype		token_type;

	(void)in_str;
	token_type = parser->tokenizer.token_type;
	if (token_type != REDIR_INFILE && token_type != REDIR_OUTFILE
		&& token_type != REDIR_HEREDOC && token_type != REDIR_APPEND)
		return (NULL);
	new_node = ast_new_node(&parser->ast, parser->tokenizer.token_type,
			parser->tokenizer.token_content);
	if (!new_node)
		return (parser_set_error(parser, MEMORY_ALLOC));
	result_str = parser_accept(parser, parser->tokenizer.token_type);
	if (!result_str)
		return (NULL);
	if (token_type == REDIR_HEREDOC)
		result_str = parser_test_redir_heredoc(parser, result_str, new_node);
	else
		result_str = parser_test_redir_inout(parser, result_str, new_node);
	parser->ast.nodes[new_node].left_node = parser->stored_node;
	parser->stored_node = new_node;
	return (result_str);
}
