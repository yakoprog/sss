/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahkaya <bahkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 00:45:52 by bahkaya           #+#    #+#             */
/*   Updated: 2026/05/09 00:45:52 by bahkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

static void	redirout_or_not(t_token *temp, t_cmd *current_cmd)
{
	temp = temp->next; // Dosya adına geç
	if (temp && temp->type == WORD)
		current_cmd->fd_out = open(temp->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}
static void	append_or_not(t_token *temp, t_cmd *current_cmd)
{
	temp = temp->next;
	if (temp && temp->type == WORD)
		current_cmd->fd_out = open(temp->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
		if (temp->type == REDIR_OUT) // > (Dosyanın içini silip yazar)
			redirout_or_not(temp, current_cmd);
		else if (temp->type == APPEND) // >> (Dosyanın sonuna ekler)
			append_or_not(temp, current_cmd);
		else if (temp->type == REDIR_IN) // < (Dosyadan okur)
			redirin_or_not(temp, current_cmd);
		// Not: HEREDOC (<<) şimdilik atlıyoruz, o ayrı bir konu.
		else if (temp->type == WORD)
	{
	word_or_not(temp, current_cmd, *i);
		i++;
	}
	if (temp)
		temp = temp->next; // Sıradaki kelimeye/işarete geç
	}
}
