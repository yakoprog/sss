/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:01:46 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:01:46 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	redirout_or_not(t_token *temp, t_cmd *current_cmd)
{
	temp = temp->next;
	if (temp && temp->type == WORD)
		current_cmd->fd_out = open(temp->value, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
}

static void	append_or_not(t_token *temp, t_cmd *current_cmd)
{
	temp = temp->next;
	if (temp && temp->type == WORD)
		current_cmd->fd_out = open(temp->value, O_WRONLY | O_CREAT | O_APPEND,
				0644);
}

static void	redirin_or_not(t_token *temp, t_cmd *current_cmd)
{
	temp = temp->next;
	if (temp && temp->type == WORD)
		current_cmd->fd_in = open(temp->value, O_RDONLY);
}

static void	word_or_not(t_token *temp, t_cmd *current_cmd, int i)
{
	current_cmd->args[i] = ft_strdup(temp->value);
}

void	pipe_or_not(t_token *temp, t_cmd *current_cmd, int *i)
{
	while (temp && temp->type != PIPE)
	{
		if (temp->type == REDIR_OUT)
			redirout_or_not(temp, current_cmd);
		else if (temp->type == APPEND)
			append_or_not(temp, current_cmd);
		else if (temp->type == REDIR_IN)
			redirin_or_not(temp, current_cmd);
		else if (temp->type == WORD)
		{
			word_or_not(temp, current_cmd, *i);
			i++;
		}
		if (temp)
			temp = temp->next;
	}
}
