#include "estr.h"

static bool	_ers_isword(char *escstr)
{
	short	wide_chr;

	wide_chr = *escstr;
	if (*escstr == '\\')
		wide_chr = *(escstr + 1) | 0x5c00;
	return ((wide_chr != ' ') && (wide_chr != 0) && (wide_chr != 0x5c00));
}

// Replaces end-word delimiter by 0 in source string to create result strings
// Don't use with const string
// First word of the result table is NULL means src str is an empty str
char	**escstr_replacesplit(char *escstr)
{
	t_xarray		xarray;
	unsigned int	new_element;
	bool			is_word_prev;
	bool			is_word;

	if (!init_xarray(&xarray, 0, sizeof(char *)))
		return (NULL);
	is_word_prev = false;
	while (*escstr)
	{
		is_word = _ers_isword(escstr);
		if (!is_word_prev && is_word)
		{
			if (!xarray_new_element(&xarray, &new_element))
				return (destroy_xarray(&xarray), NULL);
			((char **)xarray.data)[new_element] = escstr;
		}
		else if ((is_word_prev && !is_word))
			*escstr = 0;
		escstr += 1 + (*escstr == '\\');
		is_word_prev = is_word;
	}
	if (!xarray_new_element(&xarray, &new_element))
		return (destroy_xarray(&xarray), NULL);
	return (((char **)xarray.data)[new_element] = NULL, (char **)xarray.data);
}

// Copy a str doubling '\' chrs
// Don't copy if dest is NULL
// Return size in memory of the converted estr
// ! 0 IS NOT COPIED
size_t	estr_str_to_estr(char *dest, char *in_str)
{
	size_t	len;

	len = 0;
	if (!dest)
	{
		while (*in_str)
		{
			if (*in_str == '\\')
				len++;
			len++;
			in_str++;
		}
		return (len);
	}
	while (*in_str)
	{
		if (*in_str == '\\')
		{
			*dest++ = '\\';
			len++;
		}
		len++;
		*dest++ = *in_str++;
	}
	return (len);
}
