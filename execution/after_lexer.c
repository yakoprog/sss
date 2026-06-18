/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:24:34 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:24:34 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_wait(int id)
{
	int	status;

	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == 3)
			printf("Quit: 3\n");
		else if (WTERMSIG(status) == 2)
			printf("\n");
	}
	while (wait(NULL) > 0)
		;
}

int	ft_bypass(t_cmd *tmp, char ***env)
{
	int	original_out;

	if (tmp != NULL && tmp->next == NULL && is_builtin(tmp->args[0]))
	{
		original_out = dup(1);
		if (tmp->fd_out != 1)
		{
			dup2(tmp->fd_out, 1);
			close(tmp->fd_out);
		}
		execute_builtin(tmp, env);
		dup2(original_out, 1);
		close(original_out);
		return (1);
	}
	return (0);
}

void	after_lexer(t_cmd *cmds, char ***env)
{
	t_cmd	*tmp;
	int		id;
	int		prev_read_fd;

	prev_read_fd = 0;
	id = 0;
	tmp = cmds;
	if (ft_bypass(tmp, env))
		return ;
	ignore_sigint_parent();
	while (tmp != NULL)
	{
		ft_multiple_pipe(tmp, env, &id, &prev_read_fd);
		tmp = tmp->next;
	}
	ft_wait(id);
	restore_sigint_parent();
}
