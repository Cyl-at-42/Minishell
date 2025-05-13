#ifndef ENV_HANDLER_H
# define ENV_HANDLER_H

# include "xarray.h"
# include "xbuffer.h"
# include "ft_utils.h"

# define ENVHASHTABLE_SIZE 61

typedef unsigned int	t_env_node_i;

typedef struct s_env_node
{
	char			*var_str;
	unsigned int	cell_size;
	t_env_node_i	next_node;
}	t_env_node;

typedef struct s_env_handler
{
	char			**envp_origin;
	t_xbuffer		*buffer;
	t_xarray		array;
	t_env_node		*nodes;
	t_env_node_i	*hash_table;
	unsigned int	hash_table_size;
}	t_env_handler;

unsigned int	_scan_env_str(char *env_str, char **name_end,
					unsigned int *hash);
bool			_env_handler_init_nodes(t_env_handler *env_handler);
bool			_env_handler_init_modifiable(t_env_handler *env_handler);
t_env_node_i	_env_handler_find_in_list(t_env_handler *env_handler,
					t_env_node_i node, char *env_str, size_t envname_len);
bool			init_env_handler(t_env_handler *env_handler,
					unsigned int hash_table_size, char **envp);
void			destroy_env_handler(t_env_handler *env_handler);
char			**_env_handler_create_envtable_origin(
					t_env_handler *env_handler);
char			**env_handler_create_envtable(t_env_handler *env_handler);
t_env_node_i	_env_handler_prevnode(t_env_handler *env_handler,
					t_env_node_i list, t_env_node_i node);
t_env_node_i	_env_handler_list_unlink_node(t_env_handler *env_handler,
					t_env_node_i list, t_env_node_i node);
bool			env_handler_delete_env(t_env_handler *env_handler,
					char *env_name);
char			*env_handler_get_env_value(t_env_handler *env_handler,
					char *env_name);
bool			env_handler_get_env_is_null(t_env_handler *env_handler,
					char *env_name);
bool			_env_handler_set_modify(t_env_handler *env_handler,
					t_env_node_i node, char *env_str, unsigned int str_size);
bool			_env_handler_set_add(t_env_handler *env_handler, char *env_str,
					unsigned int str_size, unsigned int hash);
bool			env_handler_set(t_env_handler *env_handler, char *env_str);
bool			env_handler_setval(t_env_handler *env_handler, char *env_name,
					char *val);
#endif
