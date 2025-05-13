#include "wildcard.h"

bool	wpm_is_wildcard_pattern(char *in_escstr)
{
	while (*in_escstr)
	{
		if (*in_escstr == '*' || *in_escstr == '?')
			return (true);
		in_escstr += 1 + (*in_escstr == '\\');
	}
	return (false);
}

short	*wpm_create_pattern_wstr(char *pattern_escstr)
{
	short	*pattern_wstr;
	short	*pattern_wstrptr;

	pattern_wstr = malloc((escstr_strlen(pattern_escstr) + 1) * 2);
	if (!pattern_wstr)
		return (NULL);
	pattern_wstrptr = pattern_wstr;
	while (*pattern_escstr)
	{
		if (*pattern_escstr == '\\' )
		{
			pattern_escstr++;
			*pattern_wstrptr++ = *pattern_escstr++;
		}
		else if ((*pattern_escstr == '*') || (*pattern_escstr == '?'))
		{
			*pattern_wstrptr++ = *pattern_escstr++ | 0x2a00;
			while (*pattern_escstr == '*')
				pattern_escstr++;
		}
		else
			*pattern_wstrptr++ = *pattern_escstr++;
	}
	return (*pattern_wstrptr++ = 0, pattern_wstr);
}

static char	*_wpm_alloc(size_t *result_pattern_size,
	size_t *result_in_size, short *pattern_wstr, char *in_str)
{
	char	*new_table;

	*result_pattern_size = 1;
	while (pattern_wstr[*result_pattern_size - 1])
		(*result_pattern_size)++;
	*result_in_size = ft_strlen(in_str) + 1;
	new_table = malloc((*result_pattern_size * *result_in_size) * sizeof(char));
	if (!new_table)
		return (NULL);
	ft_memset(new_table, 0, *result_pattern_size * *result_in_size);
	new_table[1] = (*pattern_wstr == 0x2a2a);
	new_table[0] = 1;
	return (new_table);
}

// Wildcard Pattern Matching Dynamic Programming algorithm
// Returns false if mem error. If true, result is stored in *result 
bool	wpm_wildcard_pattern_match(char *result, short *pattern, char *in_str)
{
	size_t	pattern_size;
	size_t	in_size;
	size_t	i;
	size_t	j;
	char	*tbl;

	tbl = _wpm_alloc(&pattern_size, &in_size, pattern, in_str);
	if (!tbl)
		return (false);
	i = 0;
	while (++i < in_size)
	{
		j = 0;
		while (++j < pattern_size)
		{
			if ((pattern[j - 1] == 0x2a3f) || (pattern[j - 1] == in_str[i - 1]))
				tbl[i * pattern_size + j] = tbl[(i - 1) * pattern_size + j - 1];
			else if (pattern[j - 1] == 0x2a2a)
				tbl[i * pattern_size + j] = tbl[(i - 1) * pattern_size + j]
					|| tbl[i * pattern_size + j - 1];
			else
				tbl[i * pattern_size + j] = 0;
		}
	}
	return (*result = tbl[in_size * pattern_size - 1], free(tbl), true);
}
