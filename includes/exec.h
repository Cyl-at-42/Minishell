#ifndef EXEC_H
# define EXEC_H

# include <sys/types.h>

# define PIPE_READ_END_FD 0
# define PIPE_WRITE_END_FD 1

typedef struct s_exec
{
	pid_t	pid;
	int		pipe_fds[2];
	int		redir_infile_fd;
	int		redir_outfile_fd;
	int		return_code;
}	t_exec;

bool	exec_redir_infile(t_exec *exec, char *filename);
bool	exec_redir_outfile(t_exec *exec, char *filename);
bool	exec_redir_appendfile(t_exec *exec, char *filename);
bool	exec_redirect_files(t_exec *exec, bool in, bool out);
bool	exec_close_redirect_files(t_exec *exec);

#endif
