#include "parser.h"
#include "error.h"

/*
LINE	-> COMMANDS EOL
COMMANDS -> PIPELINE OPT_CONDS_CHAIN

PIPELINE -> COMMAND OPT_PIPE_CHAIN

OPT_CONDS_CHAIN	-> condop PIPELINE OPT_CONDS_CHAIN
OPT_CONDS_CHAIN	-> condop EOL !!missing cmd!!
OPT_CONDS_CHAIN	-> {}

COMMAND	-> COMMAND_ELEMENT OPT_COMMAND_ELEMENTS
COMMAND	-> ( COMMANDS )
COMMAND	-> ( COMMANDS ) REDIR OPT_REDIRS
COMMAND	-> ( COMMANDS EOL !!missing )!!

COMMAND -> COMMAND_ELT OPT_COMMAND_ELTS
OPT_COMMAND_ELTS -> COMMAND_ELT OPT_COMMAND_ELTS
OPT_COMMAND_ELTS -> {}
COMMAND_ELT -> WORD

COMMAND_ELEMENT	-> WORD
COMMAND_ELEMENT	-> REDIR
REDIR -> redirop WORD
REDIR -> redirop !!syntax error!!

OPT_PIPE_CHAIN	-> | COMMAND OPT_PIPE_CHAIN
OPT_PIPE_CHAIN	-> | EOL !!missing cmd!!
OPT_PIPE_CHAIN	-> {}

OPT_REDIRS -> REDIR OPT_REDIRS
OPT_REDIRS -> {}

OPT_COMMAND_ELEMENTS -> COMMAND_ELEMENT OPT_COMMAND_ELEMENTS
OPT_COMMAND_ELEMENTS -> {}

WORD -> WORD_ELT__OPT_WORD_ELTS
OPT_WORD_ELTS -> WORD_ELT__OPT_WORD_ELTS
OPT_WORD_ELTS -> {}
WORD_ELT -> wordchrs
WORD_ELT -> ""
WORD_ELT -> ''
*/

bool	init_parser(t_parser *parser)
{
	ft_memset(parser, 0, sizeof(t_parser));
	parser->tokenizer.buffer = create_xbuffer(0);
	if (!parser->tokenizer.buffer)
		return (false);
	if (!init_ast(&parser->ast, 64))
		return (delete_xbuffer(parser->tokenizer.buffer), false);
	return (true);
}

void	destroy_parser(t_parser *parser)
{
	delete_xbuffer(parser->tokenizer.buffer);
	parser->tokenizer.buffer = NULL;
	delete_xbuffer(parser->token_word_buffer);
	parser->token_word_buffer = NULL;
	destroy_ast(&parser->ast);
	return ;
}

char	*parser_accept(t_parser *parser, t_tokentype tokentype)
{
	char	*result_str;

	if (parser->parser_error)
		return (NULL);
	if (tokentype != parser->tokenizer.token_type)
		return (NULL);
	if (!tokentype)
		return (parser->tokenizer.end_str);
	result_str
		= tokenizer_get_token(&parser->tokenizer, parser->tokenizer.end_str);
	if (!result_str)
		return (parser->parser_error = MEMORY_ALLOC, NULL);
	if (parser->tokenizer.token_type == UNFINISHED_DOUBLE_QUOTE)
		return (parser->parser_error = MISSING_DQUOTE, NULL);
	if (parser->tokenizer.token_type == UNFINISHED_SINGLE_QUOTE)
		return (parser->parser_error = MISSING_QUOTE, NULL);
	return (result_str);
}

char	*parser_set_error(t_parser *parser, t_parse_error error)
{
	parser->parser_error = error;
	return (NULL);
}

bool	parser_parse(t_parser *parser, char *in_str)
{
	char	*result_str;

	tokenizer_get_token(&parser->tokenizer, in_str);
	result_str = parser_axiom_line(parser, in_str);
	if (!result_str)
	{
		if (!parser->parser_error)
			parser->parser_error = GENERIC_SYNTAX;
		return (false);
	}
	return (true);
}
