#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "xbuffer.h"
# include "ft_ex.h"

typedef enum e_tokentype
{
	REDIR_INFILE = '<',
	REDIR_OUTFILE = '>',
	REDIR_HEREDOC = ('<' | ('<' << 8)),
	REDIR_APPEND = ('>' | ('>' << 8)),
	COND_AND = ('&' | ('&' << 8)),
	COND_OR = ('|' | ('|' << 8)),
	PIPE = '|',
	TERM_WORD = 'w',
	ASSIGN = '=',
	FINISHED_SINGLE_QUOTE = ('\'' | ('\'' << 8)),
	FINISHED_DOUBLE_QUOTE = ('\"' | ('\"' << 8)),
	UNFINISHED_SINGLE_QUOTE = '\'',
	UNFINISHED_DOUBLE_QUOTE = '\"',
	EOL = '\0'
}	t_tokentype;

typedef struct s_tokenizer
{
	t_tokentype	token_type;
	bool		token_is_spaced;
	char		*token_content;
	t_xbuffer	*buffer;
	char		*end_str;
	char		*source;
}	t_tokenizer;

char	*tokenizer_get_token(t_tokenizer *tokenizer, char *in_str);

#endif
