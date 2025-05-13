#ifndef WILDCARD_H
# define WILDCARD_H

# include <stdlib.h>
# include <stdbool.h>
# include "estr.h"

bool	wpm_is_wildcard_pattern(char *in_escstr);
short	*wpm_create_pattern_wstr(char *pattern_escstr);
bool	wpm_wildcard_pattern_match(char *result, short *pattern, char *in_str);

#endif
