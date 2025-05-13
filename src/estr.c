#include "estr.h"

// strlen without \ taken into account (except \\)
size_t	escstr_strlen(char *in_str)
{
	size_t	len;

	len = 0;
	while (*in_str)
	{
		if (*in_str == '\\')
			in_str++;
		len++;
		in_str++;
	}
	return (len);
}

char	*escstr_escape_all(char *dest, char *in_str)
{
	char	*dest_ptr;

	dest_ptr = dest;
	while (*in_str)
	{
		*dest_ptr++ = '\\';
		*dest_ptr++ = *in_str;
		in_str++;
	}
	return (dest);
}

void	escstr_unescape_all(char *in_str)
{
	char	*dest;

	dest = in_str;
	while (*in_str)
	{
		if (*in_str == '\\')
			in_str++;
		*dest++ = *in_str++;
	}
	*dest++ = 0;
}

// copy a substring to dest inserting \ for each chr.
// Returns ptr after last copied chr
char	*escstr_esc_add_substr(char *dest, char *substr_start, char *substr_end)
{
	while (substr_start < substr_end)
	{
		*dest++ = '\\';
		*dest++ = *substr_start;
		substr_start++;
	}
	return (dest);
}

// copy a substring to dest.
// Returns ptr after last copied chr
char	*escstr_add_substr(char *dest, char *substr_start, char *substr_end)
{
	while (substr_start < substr_end)
		*dest++ = *substr_start++;
	return (dest);
}
