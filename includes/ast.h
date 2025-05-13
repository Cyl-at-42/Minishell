#ifndef AST_H
# define AST_H

# include "xarray.h"

typedef unsigned int	t_ast_node_i;

typedef struct s_astnode
{
	t_ast_node_i	left_node;
	t_ast_node_i	right_node;
	char			*str;
	unsigned int	type;
}		t_astnode;

typedef struct s_ast
{
	t_astnode		*nodes;
	t_xarray		xarray;
}		t_ast;

t_ast_node_i	ast_new_node(t_ast *ast, unsigned int type, char *str);
bool			init_ast(t_ast *ast, unsigned int capacity);
void			destroy_ast(t_ast *ast);

#endif
