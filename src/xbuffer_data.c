#include "xbuffer.h"

static t_xbuffer	*_xbuffer_add_chunk(t_xbuffer *xbuffer, size_t size)
{
	t_xbuffer	*new_chunk;
	size_t		new_chunk_size;

	new_chunk_size = size;
	if (size < xbuffer->growth_size)
		new_chunk_size = xbuffer->growth_size;
	new_chunk = create_xbuffer(new_chunk_size);
	if (!new_chunk)
		return (NULL);
	xbuffer->last_chunk->next_chunk = new_chunk;
	xbuffer->last_chunk = new_chunk;
	if (size < xbuffer->growth_size)
	{
		new_chunk_size = xbuffer->growth_size + (xbuffer->growth_size >> 1);
		if (new_chunk_size & 0xf)
		{
			new_chunk_size += 16;
			new_chunk_size >>= 4;
			new_chunk_size <<= 4;
		}
		xbuffer->growth_size = new_chunk_size ;
	}
	return (new_chunk);
}

void	*xbuffer_add_data(t_xbuffer *xbuffer, void *data, size_t size)
{
	t_xbuffer	*current_chunk;
	char		*dest;

	current_chunk = xbuffer->last_chunk;
	if (size > (current_chunk->capacity - current_chunk->used))
		current_chunk = _xbuffer_add_chunk(xbuffer, size);
	if (!current_chunk)
		return (NULL);
	dest = (char *)current_chunk + current_chunk->used + sizeof(t_xbuffer);
	ft_memcpy((void *)dest, data, size);
	current_chunk->used += size;
	return (dest);
}

void	*xbuffer_alloc(t_xbuffer *xbuffer, size_t size)
{
	t_xbuffer	*current_chunk;
	char		*dest;

	current_chunk = xbuffer->last_chunk;
	if (size > (current_chunk->capacity - current_chunk->used))
		current_chunk = _xbuffer_add_chunk(xbuffer, size);
	if (!current_chunk)
		return (NULL);
	dest = (char *)current_chunk + current_chunk->used + sizeof(t_xbuffer);
	current_chunk->used += size;
	return (dest);
}

// Copies all data from all chunks to dest
void	*xbuffer_copy_data(t_xbuffer *xbuffer, void *dest)
{
	t_xbuffer	*current_chunk;
	char		*dest_ptr;

	dest_ptr = (char *)dest;
	current_chunk = xbuffer;
	while (current_chunk)
	{
		ft_memcpy(dest_ptr, (char *)current_chunk + sizeof(t_xbuffer),
			current_chunk->used);
		dest_ptr += current_chunk->used;
		current_chunk = current_chunk->next_chunk;
	}
	return (dest);
}
