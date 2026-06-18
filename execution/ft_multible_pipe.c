/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multible_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:21:53 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 16:37:06 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	child_execute(t_cmd *cmd, char ***env)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	pipe_connection(cmd);
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
