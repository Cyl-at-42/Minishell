#include "tokenizer.h"

static bool	_tokenizer_test_chrs(t_tokenizer *tokenizer, char *in_str)
{
	t_tokentype	token_type;

	token_type = *in_str;
	if (*in_str == '>' && in_str[1] == '>')
		token_type = REDIR_APPEND;
	else if (*in_str == '<' && in_str[1] == '<')
		token_type = REDIR_HEREDOC;
	else if (*in_str == '&' && in_str[1] == '&')
		token_type = COND_AND;
	else if (*in_str == '|' && in_str[1] == '|')
		token_type = COND_OR;
	if (token_type == (t_tokentype) * in_str)
		return (false);
	tokenizer->token_type = token_type;
	tokenizer->end_str = in_str + (token_type > 255) + 1;
	return (true);
}

static char	*_tokenizer_test_quote(t_tokenizer *tokenizer, char *in_str,
	char quote_chr)
{
	char	*str_ptr;

	str_ptr = in_str;
	if (*str_ptr == quote_chr)
	{
		while (*(++str_ptr) && *str_ptr != quote_chr)
			;
		tokenizer->token_type = quote_chr;
		if (!*str_ptr)
			return (tokenizer->end_str = str_ptr, ++str_ptr);
		tokenizer->token_type = quote_chr + (quote_chr << 8);
		tokenizer->end_str = ++str_ptr;
		if (tokenizer->buffer)
		{
			tokenizer->token_content = xbuffer_alloc(tokenizer->buffer,
					str_ptr - in_str - 1);
			if (!tokenizer->token_content)
				return (NULL);
			ft_copy_substr(tokenizer->token_content, in_str + 1, str_ptr - 1);
		}
	}
	return (str_ptr);
}

static char	*_tokenizer_test_word(t_tokenizer *tokenizer, char *in_str)
{
	char	*str_ptr;
	bool	skip_dollar;

	str_ptr = in_str;
	while (*str_ptr && *str_ptr != ' ' && *str_ptr != '\t' && *str_ptr != '<'
		&& *str_ptr != '>' && *str_ptr != '|' && *str_ptr != '&'
		&& *str_ptr != '(' && *str_ptr != ')' && *str_ptr != '\''
		&& *str_ptr != '\"')
		str_ptr++;
	if (in_str == str_ptr)
		return (in_str);
	skip_dollar = (*(str_ptr - 1) == '$')
		&& (*str_ptr == '\"' || (*str_ptr == '\''));
	str_ptr -= skip_dollar;
	if (tokenizer->buffer)
	{
		tokenizer->token_content = xbuffer_alloc(tokenizer->buffer,
				str_ptr - in_str + 1);
		if (!tokenizer->token_content)
			return (NULL);
		ft_copy_substr(tokenizer->token_content, in_str, str_ptr);
	}
	tokenizer->token_type = 'w';
	str_ptr += skip_dollar;
	return (tokenizer->end_str = str_ptr, str_ptr);
}

char	*tokenizer_get_token(t_tokenizer *tokenizer, char *in_str)
{
	char	*result_str;

	tokenizer->token_is_spaced = *in_str == ' ' || *in_str == '\t';
	tokenizer->token_content = NULL;
	while (*in_str == ' ' || *in_str == '\t')
		in_str++;
	tokenizer->source = in_str;
	if (_tokenizer_test_chrs(tokenizer, in_str))
		return (in_str);
	result_str = _tokenizer_test_quote(tokenizer, in_str, '\"');
	if (result_str != in_str)
		return (in_str);
	result_str = _tokenizer_test_quote(tokenizer, in_str, '\'');
	if (result_str != in_str)
		return (in_str);
	result_str = _tokenizer_test_word(tokenizer, in_str);
	if (result_str != in_str)
		return (in_str);
	tokenizer->token_type = *in_str;
	tokenizer->end_str = in_str + 1;
	return (in_str);
}
