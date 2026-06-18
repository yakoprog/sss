#include "../minishell.h"

/* Extracted inline comments from execution/pipe_connection.c:

- Argüman yoksa çık
- DOKUNUŞ 2: Parser'da dosya açma patladıysa (fd == -1 ise) ÇALIŞTIRMA!
- Hata mesajını zaten parser'da bastığımız için burada sadece sessizce
  çıkıyoruz.
- Normal çalışıyorsa boruları bağla

These comments were moved from inside functions to avoid counting as lines
inside functions. */

void	pipe_connection(t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
		exit(0);

	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		exit(1);

	if (cmd->fd_in != 0)
	{
		dup2(cmd->fd_in, 0);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != 1)
	{
		dup2(cmd->fd_out, 1);
		close(cmd->fd_out);
	}
}
