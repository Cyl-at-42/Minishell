#ifndef XSTRING_H
# define XSTRING_H

# include <stdlib.h>
# include <stdbool.h>
# include "ft_ex.h"

typedef struct s_xstring
{
	void	*data;
	size_t	capacity;
	size_t	len;
}	t_xstring;

bool	init_xstring(t_xstring *xstring, size_t capacity);
void	destroy_xstring(t_xstring *xstring);
bool	xstring_add_substr(t_xstring *xstring, char *start, char *end);
char	*xstring_add_string(t_xstring *xstring, char *str);

#endif
