CC = cc

LIB =-L. -I includes -lreadline

CFLAGS = -g -I includes -Wall -Wextra -Werror

SRCFILES = ./src/minishell.c ./src/minishell_line.c

SRCFILES += ./src/ast.c ./src/errors.c ./src/estr.c ./src/estr_rplcsplit.c ./src/exec.c ./src/ft_ex.c ./src/ft_splitex.c ./src/get_line.c ./src/sig_handler.c ./src/tokenizer.c ./src/wildcard.c ./src/xarray.c ./src/xbuffer.c ./src/xbuffer_data.c ./src/xstring.c

SRCFILES += ./src/utils/ft_ato.c ./src/utils/ft_fuse.c ./src/utils/ft_is_str.c ./src/utils/ft_mem.c ./src/utils/ft_str.c ./src/utils/ft_str_cmp.c

SRCFILES += ./src/shell/shell.c ./src/shell/shell_exec_args_array.c ./src/shell/shell_exec_command.c ./src/shell/shell_exec_ext_command.c ./src/shell/shell_exec_groups.c ./src/shell/shell_exec_pathutils.c ./src/shell/shell_exec_pipeline.c ./src/shell/shell_exec_redirections.c ./src/shell/shell_exec_tree.c ./src/shell/shell_exec_word.c ./src/shell/shell_extend.c ./src/shell/shell_heredoc.c ./src/shell/shell_heredoc_read.c ./src/shell/shell_wildcard.c

SRCFILES += ./src/parser/parser_axiom.c ./src/parser/parser.c ./src/parser/parser_command.c ./src/parser/parser_groups.c ./src/parser/parser_redir.c ./src/parser/parser_utils.c ./src/parser/parser_word.c

SRCFILES += ./src/env/env_handler.c ./src/env/env_handler_nodes.c ./src/env/env_handler_set.c ./src/env/env_handler_table.c

SRCFILES += ./src/builtin/builtin.c ./src/builtin/echo.c ./src/builtin/env.c ./src/builtin/exit.c ./src/builtin/pwd.c ./src/builtin/unset.c src/builtin/cd.c src/builtin/export/assign.c src/builtin/export/export.c src/builtin/export/show.c

OBJECTS	= $(SRCFILES:.c=.o)

OBJDIR = objects

NAME = minishell

all: $(NAME) $(OBJECTS) 

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIB)

$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re