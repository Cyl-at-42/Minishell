#ifndef XBUFFER_H
# define XBUFFER_H

# include <stdlib.h>
# include <stdbool.h>
# include "ft_ex.h"

typedef struct s_xbuffer
{
	struct s_xbuffer	*next_chunk;
	struct s_xbuffer	*last_chunk;
	size_t				growth_size;
	size_t				capacity;
	size_t				used;
}	t_xbuffer;

t_xbuffer	*create_xbuffer(size_t capacity);
void		delete_xbuffer(t_xbuffer *xbuffer);
void		*xbuffer_add_data(t_xbuffer *xbuffer, void *data, size_t size);
void		*xbuffer_alloc(t_xbuffer *xbuffer, size_t size);
size_t		xbuffer_size(t_xbuffer *xbuffer);
void		*xbuffer_copy_data(t_xbuffer *xbuffer, void *dest);

#endif
