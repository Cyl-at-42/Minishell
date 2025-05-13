#include "minishell.h"

bool	shell_exec_conditionnal_or(t_shell *shell, t_ast_node_i node)
{
	if (shell->ast->nodes[node].left_node)
		if (!shell_exec_subtree(shell, shell->ast->nodes[node].left_node))
			return (false);
	if (shell->global_exec.return_code)
	{
		if (shell->ast->nodes[node].right_node)
			if (!shell_exec_subtree(shell, shell->ast->nodes[node].right_node))
				return (false);
	}
	return (true);
}

bool	shell_exec_conditionnal_and(t_shell *shell, t_ast_node_i node)
{
	if (shell->ast->nodes[node].left_node)
		if (!shell_exec_subtree(shell, shell->ast->nodes[node].left_node))
			return (false);
	if (!shell->global_exec.return_code)
	{
		if (shell->ast->nodes[node].right_node)
			if (!shell_exec_subtree(shell,
					shell->ast->nodes[node].right_node))
				return (false);
	}
	return (true);
}

static void	_shell_exec_subshell_child_redirs(t_shell *shell, t_shell *subshell)
{
	t_exec	*next_exec;

	if (shell->global_exec.redir_infile_fd > 0)
		subshell->global_exec.redir_infile_fd
			= dup(shell->global_exec.redir_infile_fd);
	if (shell->global_exec.redir_outfile_fd > 0)
		subshell->global_exec.redir_outfile_fd
			= dup(shell->global_exec.redir_outfile_fd);
	next_exec = shell->current_exec + 1;
	if (!shell->pipeline_execs)
		return ;
	if (shell->pipeline_exec_index)
	{
		if (subshell->global_exec.redir_infile_fd > 0)
			close(subshell->global_exec.redir_infile_fd);
		subshell->global_exec.redir_infile_fd
			= dup(shell->current_exec->pipe_fds[PIPE_READ_END_FD]);
	}
	if (shell->pipeline_exec_index < (shell->pipeline_size - 1))
	{
		if (subshell->global_exec.redir_outfile_fd > 0)
			close(subshell->global_exec.redir_outfile_fd);
		subshell->global_exec.redir_outfile_fd
			= dup(next_exec->pipe_fds[PIPE_WRITE_END_FD]);
	}
}

static bool	_shell_exec_subshell_child(t_shell *shell, t_shell *subshell,
	t_ast_node_i node)
{
	init_shell(subshell, shell->env_handler, shell->pwd,
		shell->global_exec.return_code);
	subshell->ast = shell->ast;
	subshell->global_exec.pid = -1;
	subshell->global_exec.return_code = 1;
	subshell->generated_args_buffer = create_xbuffer(0);
	if (!subshell->generated_args_buffer)
		return (false);
	subshell->current_exec = &subshell->global_exec;
	_shell_exec_subshell_child_redirs(shell, subshell);
	if ((subshell->global_exec.redir_infile_fd == -1)
		|| (subshell->global_exec.redir_outfile_fd == -1))
		return (perror("dup"), destroy_shell(subshell), false);
	destroy_shell(shell);
	subshell->global_exec.return_code = 0;
	if (shell->ast->nodes[node].right_node
		&& (!shell_exec_subtree(subshell, shell->ast->nodes[node].right_node)
			|| (subshell->global_exec.return_code != 0)))
		return (destroy_shell(subshell), false);
	delete_xbuffer(subshell->generated_args_buffer);
	subshell->generated_args_buffer = NULL;
	subshell->current_exec = &subshell->exec;
	shell_exec_subtree(subshell, shell->ast->nodes[node].left_node);
	return (shell->global_exec.return_code = destroy_shell(subshell), false);
}

// fonction failure
// Right node first: redirs
// Left node: execs
bool	shell_exec_subshell(t_shell *shell, t_ast_node_i node)
{
	t_shell	subshell;
	int		status;
	pid_t	newpid;

	newpid = fork();
	if (newpid == -1)
		return (write_error_strs(2, "fork: ", ""), false);
	if (newpid == 0)
		return (shell->global_exec.pid = -1,
			_shell_exec_subshell_child(shell, &subshell, node), false);
	shell->current_exec->pid = newpid;
	if (shell->pipeline_execs)
		return (true);
	status = 0;
	waitpid(newpid, &status, 0);
	shell->global_exec.return_code = WEXITSTATUS(status);
	return (true);
}
