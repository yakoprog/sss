#include "../minishell.h"

/* Extracted inline comments from execution/ft_multible_pipe.c:

- env zaten 3 yıldızlı olduğu için direkt gönderiyoruz
- built-in'in bıraktığı son durum ile çıkış kodunu belirliyoruz
- dış programlar **env bekler, bu yüzden *env gönderiyoruz
- execve ancak başarısız olursa buraya düşer
- başarısız komutlar için uygun kodlar 126/127 kullanılıyor
- çocuk proses, boru bağlantısı ve dosya tanımlayıcı yönetimi
- prev_read_fd her komutta bir önceki borunun okuma ucunu tutar
- son süreç, girdi ve çıktıyı pipe aracılığıyla bağlamak için çalışır

These comments were moved from inside functions to avoid counting as lines
inside functions. */

static void	child_execute(t_cmd *cmd, char ***env)
{
	char	*path;

	pipe_connection(cmd);
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		exit(g_exit_status);
	}
	path = put_command(cmd->args[0], *env);
	if (!path)
	{
		print_error(cmd->args[0], "command not found", 127);
		exit(127);
	}
	execve(path, cmd->args, *env);
	print_error(cmd->args[0], strerror(errno), 126);
	exit(126);
}

static void	ft_dup_except_last(int prev_read_fd)
{
	dup2(prev_read_fd, 0);
	close(prev_read_fd);
}

static void	ft_dup_last(int *fd)
{
	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
}

void	ft_multiple_pipe(t_cmd *tmp, char ***env, int *id, int *prev_read_fd)
{
	int	fd[2];
	if (tmp->next != NULL)
		pipe(fd);
	*id = fork();
	if (*id == 0)
	{
		if (*prev_read_fd != 0)
			ft_dup_except_last(*prev_read_fd);
		if (tmp->next != NULL)
			ft_dup_last(fd);
		child_execute(tmp, env);
	}
	else
	{
		if (*prev_read_fd != 0)
			close(*prev_read_fd);
		if (tmp->next != NULL)
		{
			close(fd[1]);
			*prev_read_fd = fd[0];
		}
	}
}
