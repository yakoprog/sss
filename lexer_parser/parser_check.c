/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 13:45:34 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 13:45:34 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void redirout_or_not(t_token **temp, t_cmd *current_cmd)
{
	char	*clean_path;

	*temp = (*temp)->next;
	if (*temp && (*temp)->type == WORD)
	{
		clean_path = remove_quotes(ft_strdup((*temp)->value));
		current_cmd->fd_out = open(clean_path,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (current_cmd->fd_out == -1)
			print_error(clean_path, strerror(errno), 1);
		free(clean_path);
	}
}

static void	append_or_not(t_token **temp, t_cmd *current_cmd)
{
	char	*clean_path;

	*temp = (*temp)->next;
	if (*temp && (*temp)->type == WORD)
	{
		clean_path = remove_quotes(ft_strdup((*temp)->value));
		current_cmd->fd_out = open(clean_path,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (current_cmd->fd_out == -1)
			print_error(clean_path, strerror(errno), 1);
		free(clean_path);
	}
}

static void	redirin_or_not(t_token **temp, t_cmd *current_cmd)
{
	char	*clean_path;

	*temp = (*temp)->next;
	if (*temp && (*temp)->type == WORD)
	{
		clean_path = remove_quotes(ft_strdup((*temp)->value));
		current_cmd->fd_in = open(clean_path, O_RDONLY);
		if (current_cmd->fd_in == -1)
			print_error(clean_path, strerror(errno), 1);
		free(clean_path);
	}
}

static void	word_or_not(t_token *temp, t_cmd *current_cmd, int i)
{
	current_cmd->args[i] = ft_strdup(temp->value);
	current_cmd->arg_quoted[i] = (temp->quote_type != 0);
}

static void	skip_to_pipe(t_token **temp)
{
	while (*temp && (*temp)->type != PIPE)
		*temp = (*temp)->next;
}

void	pipe_or_not(t_token **temp, t_cmd *current_cmd, int *i)
{
	while (*temp && (*temp)->type != PIPE)
	{
		if ((*temp)->type == REDIR_OUT)
		{
			redirout_or_not(temp, current_cmd);
			if (current_cmd->fd_out == -1)
				return (skip_to_pipe(temp));
		}
		else if ((*temp)->type == APPEND)
		{
			append_or_not(temp, current_cmd);
			if (current_cmd->fd_out == -1)
				return (skip_to_pipe(temp));
		}
		else if ((*temp)->type == REDIR_IN)
		{
			redirin_or_not(temp, current_cmd);
			if (current_cmd->fd_in == -1)
				return (skip_to_pipe(temp));
		}
		else if ((*temp)->type == HEREDOC)
		{
			*temp = (*temp)->next;
			if (*temp && (*temp)->type == WORD)
				current_cmd->fd_in = handle_heredoc((*temp)->value);
			if (current_cmd->fd_in == -1)
				return (skip_to_pipe(temp));
		}
		else if ((*temp)->type == WORD)
		{
			word_or_not(*temp, current_cmd, *i);
			(*i)++;
		}
		if (*temp)
			*temp = (*temp)->next;
	}
}
