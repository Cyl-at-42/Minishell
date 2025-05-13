#include "xarray.h"

// = xarray (eXtendable array) ================================================

bool	init_xarray(t_xarray *xarray, unsigned int capacity,
	unsigned int struct_size)
{
	if (capacity < 4)
		capacity = 4;
	xarray->capacity = capacity;
	xarray->struct_size = struct_size;
	xarray->used = 0;
	xarray->growth_size = capacity;
	xarray->data = malloc(capacity * struct_size);
	return (xarray->data != NULL);
}

void	destroy_xarray(t_xarray *xarray)
{
	free(xarray->data);
	xarray->data = NULL;
}

bool	xarray_new_element(t_xarray *xarray, unsigned int *new_element_ptr)
{
	size_t			struct_size;
	unsigned long	new_capacity;

	if (xarray->capacity == XARRAY_CAPACITYMAX || !xarray->data)
		return (false);
	struct_size = xarray->struct_size;
	if (xarray->used == xarray->capacity)
	{
		new_capacity = xarray->capacity + xarray->growth_size;
		if (new_capacity > XARRAY_CAPACITYMAX)
			new_capacity = XARRAY_CAPACITYMAX;
		if (!ft_realloc(&xarray->data, xarray->capacity * struct_size,
				new_capacity * struct_size))
			return (false);
		xarray->growth_size += xarray->growth_size >> 1;
		xarray->capacity = new_capacity;
	}
	if (new_element_ptr)
		*new_element_ptr = xarray->used;
	xarray->used++;
	return (true);
}
