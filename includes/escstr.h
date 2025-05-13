#ifndef ESCSTR_H
# define ESCSTR_H

# include <stdbool.h>
# include "xarray.h"

char	**escstr_replacesplit(char *escstr);
size_t	escstr_strlen(char *in_str);
char	*escstr_escape_all(char *dest, char *in_str);
void	escstr_unescape_all(char *in_str);
char	*escstr_esc_add_substr(
			char *dest, char *substr_start, char *substr_end);
char	*escstr_add_substr(char *dest, char *substr_start, char *substr_end);

#endif
