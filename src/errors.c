#include "minishell.h"
#include <stdarg.h>

void	write_error_strs(int str_count, ...)
{
	va_list	arglist;
	char	*error_str;

	va_start(arglist, str_count);
	while (str_count)
	{
		error_str = va_arg(arglist, char *);
		if (*error_str == 0 && (str_count == 1))
		{
			write(2, " ", 1);
			perror("");
		}
		else
			write(2, error_str, ft_strlen(error_str));
		str_count--;
	}
	va_end(arglist);
}

char	*get_word_end(t_tokenizer *tokenizer, char *in_str)
{
	tokenizer_get_token(tokenizer, in_str);
	if (!tokenizer->token_type)
		return (in_str);
	if (tokenizer->token_type == 'w'
		|| tokenizer->token_type == FINISHED_SINGLE_QUOTE
		|| tokenizer->token_type == FINISHED_DOUBLE_QUOTE)
	{
		while (1)
		{
			if (!tokenizer->token_type)
				return (in_str);
			if (tokenizer->token_is_spaced)
				return (in_str);
			if (tokenizer->token_type != 'w'
				&& tokenizer->token_type != FINISHED_SINGLE_QUOTE
				&& tokenizer->token_type != FINISHED_DOUBLE_QUOTE)
				return (in_str);
			in_str = tokenizer->end_str;
			tokenizer_get_token(tokenizer, in_str);
		}
	}
	return (tokenizer->end_str);
}

static void	_minishell_write_parser_syntaxerror(t_minishell *minishell,
	t_parser *parser)
{
	char	*src;

	write_error_strs(2, APP_NAME,
		": syntax error near unexpected token `");
	src = parser->tokenizer.source;
	delete_xbuffer(parser->tokenizer.buffer);
	parser->tokenizer.buffer = NULL;
	if (src)
	{
		if (*src)
			write(2, src, get_word_end(&parser->tokenizer, src) - src);
		else
			write(2, "newline", 7);
	}
	write(2, "'\n", 2);
	minishell->prev_retcode = 2;
}

void	minishell_write_parser_errors(t_minishell *minishell,
	t_parser *parser)
{
	if (parser->parser_error == INTERNAL)
		write_error_strs(2, APP_NAME, ": Internal error\n");
	else if (parser->parser_error == MEMORY_ALLOC)
		write_error_strs(2, APP_NAME, ": Memory allocation error\n");
	else if (parser->parser_error == HEREDOC)
		write_error_strs(2, APP_NAME, ": Heredoc error\n");
	else if (parser->parser_error == MISSING_PARENTHESIS)
		write_error_strs(2, APP_NAME, ": Missing )\n");
	else if (parser->parser_error == MISSING_DQUOTE)
		write_error_strs(2, APP_NAME, ": Missing \"\n");
	else if (parser->parser_error == MISSING_QUOTE)
		write_error_strs(2, APP_NAME, ": Missing \'\n");
	else if (parser->parser_error == MISSING_COMMAND)
		write_error_strs(2, APP_NAME, ": Missing command\n");
	else if (parser->parser_error == GENERIC_SYNTAX)
		_minishell_write_parser_syntaxerror(minishell, parser);
}
