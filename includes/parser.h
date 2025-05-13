#ifndef PARSER_H
# define PARSER_H

# include "ast.h"
# include "xbuffer.h"
# include "xstring.h"
# include "tokenizer.h"
# include "shell.h"

//# define PARSER_NODE_TYPE_WORD_ELTS ('w' | (',' << 8)) 0x2c77
# define PARSER_NODE_TYPE_WORD_ELTS 0x2c77

typedef enum e_parse_error
{
	OK,
	INTERNAL,
	MEMORY_ALLOC,
	GENERIC_SYNTAX,
	HEREDOC,
	HEREDOC_CLEAN,
	MISSING_PARENTHESIS,
	MISSING_QUOTE,
	MISSING_DQUOTE,
	MISSING_COMMAND
}	t_parse_error;

typedef struct s_shell	t_shell;

typedef struct s_parser
{
	t_ast			ast;
	t_tokenizer		tokenizer;
	t_parse_error	parser_error;
	t_ast_node_i	stored_node;
	t_xbuffer		*token_word_buffer;
	int				(*get_heredoc)(t_shell *, char *, char *, t_xstring *);
	void			*app_info;
}	t_parser;

bool	init_parser(t_parser *parser);
void	destroy_parser(t_parser *parser);
char	*parser_accept(t_parser *parser, t_tokentype tokentype);
char	*parser_set_error(t_parser *parser, t_parse_error error);
bool	parser_parse(t_parser *parser, char *in_str);
char	*parser_test_pipeline(t_parser *parser, char *in_str);
char	*parser_test_command(t_parser *parser, char *in_str);
char	*parser_test_commands(t_parser *parser, char *in_str);
char	*parser_axiom_line(t_parser *parser, char *in_str);
char	*parser_test_command_element(t_parser *parser, char *in_str);
char	*parser_test_word_element(t_parser *parser, char *in_str);
char	*parser_test_word(t_parser *parser, char *in_str);
char	*parser_test_redir(t_parser *parser, char *in_str);
char	*parser_test_opt_redirs(t_parser *parser, char *in_str);
char	*parser_test_subshell(t_parser *parser, char *in_str);
char	*parser_test_opt_conditionnal_chain(t_parser *parser, char *in_str);
char	*parser_test_command2(t_parser *parser, char *in_str);
bool	parser_tokenjoin_unquoted(t_parser *parser, t_ast_node_i node);
bool	parser_is_quoted_word(t_parser *parser, t_ast_node_i node);

#endif
