#include "minishell.h"

bool	shell_exec_subtree(t_shell *shell, t_ast_node_i node)
{
	unsigned int	node_type;

	node_type = shell->ast->nodes[node].type;
	if (node_type == 'x')
		return (shell_exec_command(shell, node));
	else if (node_type == ',')
		return (shell_exec_children(shell, node));
	else if (node_type == COND_AND)
		return (shell_exec_conditionnal_and(shell, node));
	else if (node_type == COND_OR)
		return (shell_exec_conditionnal_or(shell, node));
	else if (node_type == '|')
		return (shell_exec_pipe(shell, node));
	else if (node_type == '(')
		return (shell_exec_subshell(shell, node));
	else if (node_type == '<' || node_type == '>'
		|| node_type == REDIR_HEREDOC || node_type == REDIR_APPEND)
		return (shell_exec_redirs(shell, node));
	else if (node_type == 'w' || node_type == FINISHED_DOUBLE_QUOTE
		|| node_type == FINISHED_SINGLE_QUOTE
		|| node_type == PARSER_NODE_TYPE_WORD_ELTS)
		return (shell_exec_word_elements(shell, node));
	return (false);
}

bool	shell_exec_children(t_shell *shell, t_ast_node_i node)
{
	if (shell->ast->nodes[node].left_node)
		if (!shell_exec_subtree(shell, shell->ast->nodes[node].left_node))
			return (false);
	if (shell->ast->nodes[node].right_node)
		if (!shell_exec_subtree(shell, shell->ast->nodes[node].right_node))
			return (false);
	return (true);
}

void	shell_clean_tree_files(t_shell *shell, t_ast_node_i node)
{
	if (shell->ast->nodes[node].type == REDIR_HEREDOC)
	{
		if (unlink(shell->ast->nodes[node].str) == -1)
			write_error_strs(2, "unlink:", "");
	}
	if (shell->ast->nodes[node].left_node)
		shell_clean_tree_files(shell, shell->ast->nodes[node].left_node);
	if (shell->ast->nodes[node].right_node)
		shell_clean_tree_files(shell, shell->ast->nodes[node].right_node);
}
