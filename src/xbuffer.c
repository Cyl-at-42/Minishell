#include "xbuffer.h"

// = xbuffer (eXtendable buffer) ==============================================

t_xbuffer	*create_xbuffer(size_t capacity)
{
	t_xbuffer	*new_xbuffer;

	new_xbuffer = (t_xbuffer *)malloc(capacity + sizeof(t_xbuffer));
	if (!new_xbuffer)
		return (NULL);
	new_xbuffer->next_chunk = NULL;
	new_xbuffer->last_chunk = new_xbuffer;
	new_xbuffer->capacity = capacity;
	if (capacity < 32)
		capacity = 32;
	new_xbuffer->growth_size = capacity;
	new_xbuffer->used = 0;
	return (new_xbuffer);
}

void	delete_xbuffer(t_xbuffer *xbuffer)
{
	t_xbuffer	*next_chunk;

	while (xbuffer)
	{
		next_chunk = xbuffer->next_chunk;
		free(xbuffer);
		xbuffer = next_chunk;
	}
}

size_t	xbuffer_size(t_xbuffer *xbuffer)
{
	size_t		result_size;
	t_xbuffer	*current_chunk;

	result_size = 0;
	current_chunk = xbuffer;
	while (current_chunk)
	{
		result_size += current_chunk->used;
		current_chunk = current_chunk->next_chunk;
	}
	return (result_size);
}
