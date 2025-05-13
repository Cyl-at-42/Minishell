#include <stdlib.h>

static unsigned int	count_words(char const *str, char delim)
{
	unsigned int	wordcount;
	int				isword;
	int				iswordprev;

	wordcount = 0;
	iswordprev = 0;
	while (*str)
	{
		isword = (*str && *str != delim);
		if (isword && !iswordprev)
			wordcount++;
		str++;
		iswordprev = isword;
	}
	return (wordcount);
}

static char	*alloc_word(char *wordstart, char *wordend, size_t extrachars)
{
	unsigned int	wordlen;
	char			*newword;
	char			*wordptr;

	wordlen = wordend - wordstart;
	newword = (char *)malloc((wordlen + extrachars + 1) * sizeof(char));
	if (!newword)
		return (NULL);
	wordptr = newword;
	while (wordstart < wordend)
		*wordptr++ = *wordstart++;
	*wordptr = 0;
	return (newword);
}

static unsigned int	alloc_words(char **table, char *str, char delim,
	size_t extrachars)
{
	unsigned int	wordindex;
	int				isword;
	int				iswordprev;
	char			*wordstart;
	int				endofstring;

	wordindex = 0;
	wordstart = str;
	iswordprev = 0;
	endofstring = 0;
	while (!endofstring)
	{
		isword = (*str && *str != delim);
		if (isword && !iswordprev)
			wordstart = str;
		if (!isword && iswordprev)
		{
			table[wordindex++] = alloc_word(wordstart, str, extrachars);
			if (!table[wordindex - 1])
				return (wordindex - 1);
		}
		endofstring = (*str++ == 0);
		iswordprev = isword;
	}
	return (wordindex);
}

static char	**free_all(char **table, unsigned int wordcount)
{
	unsigned int	i;

	i = 0;
	while (i < wordcount)
		free(table[i++]);
	free(table);
	return (NULL);
}

char	**ft_split_ex(char const *s, char c, size_t extrachars)
{
	unsigned int	wordcount;
	unsigned int	allocated_wordcount;
	char			**ret_table;

	wordcount = count_words(s, c);
	ret_table = (char **)malloc((wordcount + 1) * sizeof(char *));
	if (!ret_table)
		return (NULL);
	if (wordcount)
	{
		allocated_wordcount = alloc_words(ret_table, (char *)s, c, extrachars);
		if (allocated_wordcount != wordcount)
			return (free_all(ret_table, allocated_wordcount));
	}
	ret_table[wordcount] = 0;
	return (ret_table);
}
