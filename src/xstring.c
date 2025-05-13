#include "xstring.h"

// = xstring (eXtendable string) ==============================================

bool	init_xstring(t_xstring *xstring, size_t capacity)
{
	if (capacity < 32)
		capacity = 32;
	xstring->data = malloc(capacity);
	if (!xstring->data)
		return (false);
	xstring->capacity = capacity;
	xstring->len = 0;
	return (true);
}

void	destroy_xstring(t_xstring *xstring)
{
	free(xstring->data);
	xstring->data = NULL;
}

static bool	_xstring_growth(t_xstring *xstring, size_t extra_size)
{
	size_t	new_size;

	new_size = xstring->capacity + (xstring->capacity >> 1) + extra_size;
	if (!ft_realloc(&xstring->data, xstring->capacity, new_size))
		return (false);
	xstring->capacity = new_size;
	return (true);
}

bool	xstring_add_substr(t_xstring *xstring, char *start, char *end)
{
	size_t	substr_size;
	char	*ptr;

	substr_size = end - start;
	if (!substr_size)
		return (true);
	if (substr_size > (xstring->capacity - xstring->len))
		if (!_xstring_growth(xstring, substr_size + 1))
			return (false);
	ptr = (char *)xstring->data + xstring->len;
	ft_memcpy(ptr, start, substr_size);
	ptr[substr_size] = 0;
	xstring->len += substr_size;
	return (true);
}

// Returns the ptr of inserted string
char	*xstring_add_string(t_xstring *xstring, char *str)
{
	size_t	string_size;
	char	*ptr;

	ptr = str;
	while (*ptr++)
		;
	string_size = ptr - str;
	if (string_size > (xstring->capacity - (xstring->len + 1)))
		if (!_xstring_growth(xstring, string_size))
			return (NULL);
	ptr = (char *)xstring->data + xstring->len;
	ft_memcpy(ptr, str, string_size);
	xstring->len += (string_size - 1);
	return (ptr);
}
