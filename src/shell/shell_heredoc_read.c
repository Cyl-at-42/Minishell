#include "minishell.h"

static bool	_shell_get_hd_process_write(t_shell *shell, t_xstring *exp_string,
	int w_fd, char *input)
{
	if (exp_string)
	{
		exp_string->len = 0;
		if (!shell_extend_string(shell, exp_string, input))
			return (false);
		if (write(w_fd, exp_string->data, exp_string->len) == -1)
			return (false);
	}
	else
		if (write(w_fd, input, ft_strlen(input)) == -1)
			return (false);
	if (write(w_fd, "\n", 1) == -1)
		return (false);
	return (true);
}

bool	shell_get_hd_process2(t_shell_heredoc *hd, char *limiter, int echo_fd)
{
	size_t	limiter_len;
	char	*input;

	limiter_len = ft_strlen(limiter);
	while (1)
	{
		if (!readline_ext(&input, "\033[1mheredoc> \033[0m", echo_fd))
			break ;
		if (!input)
		{
			write_error_strs(4, APP_NAME, ": warning: here-document"
				" delimited by end-of-file (wanted `", limiter, "')\n");
			break ;
		}
		if (!ft_strncmp(input, limiter, limiter_len) && !input[limiter_len])
			break ;
		if (!_shell_get_hd_process_write(hd->shell, hd->exp_string,
				hd->pipe_fds[1], input))
		{
			perror("get_hd_process_write");
			break ;
		}
		free(input);
	}
	return (free(input), close(hd->pipe_fds[PIPE_WRITE_END_FD]), true);
}

bool	shell_get_hd_process(t_shell_heredoc *hd, char *limiter, int echo_fd)
{
	heredoc_sig(hd);
	close(hd->pipe_fds[PIPE_READ_END_FD]);
	return (shell_get_hd_process2(hd, limiter, echo_fd));
}
