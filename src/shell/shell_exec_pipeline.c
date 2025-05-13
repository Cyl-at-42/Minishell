#include "minishell.h"

static unsigned int	_shell_get_pipeline_size(t_shell *shell, t_ast_node_i node)
{
	unsigned int	size;

	size = 0;
	if (shell->ast->nodes[node].type != '|')
		return (1);
	size++;
	size += _shell_get_pipeline_size(shell, shell->ast->nodes[node].right_node);
	return (size);
}

void	shell_close_pipes_and_files(t_shell *shell)
{
	unsigned int	pipe_index;

	pipe_index = 0;
	while (pipe_index < shell->pipeline_size)
	{
		ft_safe_close(&shell->pipeline_execs[pipe_index].pipe_fds[0]);
		ft_safe_close(&shell->pipeline_execs[pipe_index].pipe_fds[1]);
		ft_safe_close(&shell->pipeline_execs[pipe_index].redir_infile_fd);
		ft_safe_close(&shell->pipeline_execs[pipe_index].redir_outfile_fd);
		pipe_index++;
	}
}

void	shell_wait_pipeline(t_shell *shell)
{
	unsigned int	i;
	int				status;

	i = 0;
	while (i < shell->pipeline_size)
	{
		status = 1 << 8;
		if (shell->pipeline_execs[i].pid)
		{
			waitpid(shell->pipeline_execs[i].pid, &status, 0);
			shell->pipeline_execs[i].return_code = WEXITSTATUS(status);
		}
		i++;
	}
}

bool	shell_exec_pipeline(t_shell *shell, t_ast_node_i node)
{
	size_t	size;
	bool	r;

	shell->pipeline_size = _shell_get_pipeline_size(shell, node);
	size = shell->pipeline_size * sizeof(t_exec);
	shell->pipeline_execs = malloc(size);
	if (!shell->pipeline_execs)
		return (false);
	ft_memset(shell->pipeline_execs, 0, size);
	shell->pipeline_exec_index = 0;
	shell->current_exec = shell->pipeline_execs;
	r = shell_exec_pipe(shell, node);
	shell_close_pipes_and_files(shell);
	if (r)
	{
		shell_wait_pipeline(shell);
		shell->global_exec.return_code
			= shell->pipeline_execs[shell->pipeline_size - 1].return_code;
	}
	free(shell->pipeline_execs);
	shell->pipeline_execs = NULL;
	return (shell->current_exec = &shell->exec, r);
}

bool	shell_exec_pipe(t_shell *shell, t_ast_node_i node)
{
	t_exec	*exec;
	t_exec	*exec_prev;

	if (!shell->pipeline_execs)
		return (shell_exec_pipeline(shell, node));
	exec = shell->current_exec;
	if (shell->pipeline_exec_index >= 2)
	{
		exec_prev = exec - 1;
		ft_safe_close(&exec_prev->pipe_fds[0]);
		ft_safe_close(&exec_prev->pipe_fds[1]);
	}
	exec++;
	if (pipe(exec->pipe_fds) == -1)
		return (write_error_strs(2, "pipe: ", ""), false);
	shell->current_exec = &shell->pipeline_execs[shell->pipeline_exec_index];
	if (!shell_exec_subtree(shell, shell->ast->nodes[node].left_node))
		return (false);
	shell->pipeline_exec_index++;
	shell->current_exec = &shell->pipeline_execs[shell->pipeline_exec_index];
	if (!shell_exec_subtree(shell, shell->ast->nodes[node].right_node))
		return (false);
	return (true);
}
