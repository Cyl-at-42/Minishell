#include "minishell.h"

bool	exec_redir_infile(t_exec *exec, char *filename)
{
	if (exec->redir_infile_fd > 0)
		close(exec->redir_infile_fd);
	exec->redir_infile_fd = open(filename, O_RDONLY);
	if (exec->redir_infile_fd >= 0)
		return (true);
	exec->return_code = 1;
	return (false);
}

bool	exec_redir_outfile(t_exec *exec, char *filename)
{
	if (exec->redir_outfile_fd > 0)
		close(exec->redir_outfile_fd);
	exec->redir_outfile_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (exec->redir_outfile_fd >= 0)
		return (true);
	exec->return_code = 1;
	return (false);
}

bool	exec_redir_appendfile(t_exec *exec, char *filename)
{
	if (exec->redir_outfile_fd > 0)
		close(exec->redir_outfile_fd);
	exec->redir_outfile_fd = open(filename,
			O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (exec->redir_outfile_fd >= 0)
		return (true);
	exec->return_code = 1;
	return (false);
}

bool	exec_redirect_files(t_exec *exec, bool in, bool out)
{
	if (exec->redir_outfile_fd && out)
		if (dup2(exec->redir_outfile_fd, STDOUT_FILENO) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
	if (exec->redir_infile_fd && in)
		if (dup2(exec->redir_infile_fd, STDIN_FILENO) == -1)
			return (write_error_strs(2, "dup2:", ""), false);
	return (true);
}

bool	exec_close_redirect_files(t_exec *exec)
{
	ft_safe_close(&exec->redir_infile_fd);
	ft_safe_close(&exec->redir_outfile_fd);
	return (true);
}
