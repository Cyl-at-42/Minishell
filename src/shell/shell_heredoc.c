#include "minishell.h"

static int	_shell_get_hd_open_tmpfile(char *out_filename)
{
	ssize_t	bytes_read;
	int		fd;
	int		i;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (-1);
	ft_memcpy(out_filename, "/tmp/ms_", 8);
	bytes_read = read(fd, &out_filename[8], 64);
	if (bytes_read == -1)
		return (close(fd), -1);
	close(fd);
	out_filename[72] = 0;
	i = 8;
	while (i < 72)
	{
		out_filename[i] = TFNCS[(unsigned char) out_filename[i] % 62];
		i++;
	}
	return (open(out_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644));
}

static int	_shell_get_hd_write(char *out_filename, int *pipe_fds)
{
	char	buffer[32];
	ssize_t	bytes_read;
	int		tmpfile_fd;

	tmpfile_fd = _shell_get_hd_open_tmpfile(out_filename);
	if (tmpfile_fd == -1)
		return (-1);
	while (1)
	{
		bytes_read = read(pipe_fds[PIPE_READ_END_FD], buffer, 32);
		if (bytes_read <= 0)
			break ;
		if (write(tmpfile_fd, buffer, bytes_read) == -1)
			return (close(tmpfile_fd), -1);
	}
	close(tmpfile_fd);
	if (bytes_read < 0)
		return (-1);
	return (0);
}

static void	_clean(t_xstring *exp_string, int *pipe_fds, char *msg)
{
	if (exp_string)
		destroy_xstring(exp_string);
	if (pipe_fds[0])
	{
		close(pipe_fds[PIPE_READ_END_FD]);
		pipe_fds[PIPE_READ_END_FD] = 0;
	}
	if (pipe_fds[1])
	{
		close(pipe_fds[PIPE_WRITE_END_FD]);
		pipe_fds[PIPE_WRITE_END_FD] = 0;
	}
	if (msg)
	{
		if (*msg)
			write_error_strs(2, "get_heredoc: ", "");
		else
			write_error_strs(3, "get_heredoc: ", msg, "");
	}
}

static void	_init_heredoc(t_shell_heredoc *heredoc, t_shell *shell,
	t_xstring *exp_string)
{
	heredoc->error = 0;
	heredoc->shell = shell;
	heredoc->exp_string = exp_string;
}

int	shell_get_heredoc(t_shell *shell, char *dest, char *limiter,
	t_xstring *exp_string)
{
	t_shell_heredoc	heredoc;

	_init_heredoc(&heredoc, shell, exp_string);
	if (exp_string && !init_xstring(exp_string, 42))
		return (write_error_strs(2, "get_heredoc: ", ""), -1);
	if (pipe(heredoc.pipe_fds) == -1)
		return (_clean(exp_string, NULL, "pipe: "), -1);
	heredoc.new_pid = fork();
	if (heredoc.new_pid == -1)
		return (_clean(exp_string, NULL, "fork: "), -1);
	if (!heredoc.new_pid)
		return (shell->global_exec.pid = -1,
			shell_get_hd_process(&heredoc, limiter, shell->echo_fd),
			_clean(exp_string, heredoc.pipe_fds, NULL), 1);
	close(heredoc.pipe_fds[PIPE_WRITE_END_FD]);
	heredoc.error = _shell_get_hd_write(dest, heredoc.pipe_fds);
	close(heredoc.pipe_fds[PIPE_READ_END_FD]);
	heredoc.waitpidret = waitpid(heredoc.new_pid, &heredoc.status, 0);
	if (heredoc.error)
		write_error_strs(2, "get_heredoc:", "");
	if (exp_string)
		destroy_xstring(exp_string);
	if (heredoc.waitpidret == -1)
		heredoc.error = -2;
	return (heredoc.error);
}
