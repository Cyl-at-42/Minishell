#ifndef XARRAY_H
# define XARRAY_H

# include <stdlib.h>
# include <stdbool.h>
# include "ft_ex.h"

typedef struct s_xarray
{
	unsigned int	capacity;
	unsigned int	used;
	unsigned int	struct_size;
	unsigned int	growth_size;
	void			*data;
}	t_xarray;

# define XARRAY_CAPACITYMAX 0xffffffff

bool	init_xarray(t_xarray *xarray, unsigned int capacity,
			unsigned int struct_size);
void	destroy_xarray(t_xarray *xarray);
bool	xarray_new_element(t_xarray *xarray, unsigned int *new_element_ptr);

#endif
