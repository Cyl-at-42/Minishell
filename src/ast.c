#include "ast.h"

t_ast_node_i	ast_new_node(t_ast *ast, unsigned int type, char *str)
{
	unsigned int	new_node;

	if (!xarray_new_element(&ast->xarray, &new_node))
		return (0);
	ast->nodes = (t_astnode *)ast->xarray.data;
	ast->nodes[new_node].left_node = 0;
	ast->nodes[new_node].right_node = 0;
	ast->nodes[new_node].str = str;
	ast->nodes[new_node].type = type;
	return (new_node);
}

bool	init_ast(t_ast *ast, unsigned int capacity)
{
	if (!capacity)
		return (false);
	if (!init_xarray(&ast->xarray, capacity, sizeof(t_astnode)))
		return (ast->nodes = NULL, false);
	ast->nodes = (t_astnode *)ast->xarray.data;
	ast_new_node(ast, 0, NULL);
	return (true);
}

void	destroy_ast(t_ast *ast)
{
	destroy_xarray(&ast->xarray);
	ast->nodes = NULL;
}

void	ast_set_node_children(t_ast *ast, t_ast_node_i node,
	t_ast_node_i left, t_ast_node_i right)
{
	ast->nodes[node].left_node = left;
	ast->nodes[node].right_node = right;
}
