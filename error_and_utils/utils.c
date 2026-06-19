/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:28:50 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:28:50 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
 
static void	close_cmd_fds(t_cmd *cmd)
{
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
}
 
void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;
 
	while (cmds != NULL)
	{
		tmp = cmds;
		cmds = cmds->next;
		close_cmd_fds(tmp);
		if (tmp->args != NULL)
		{
			i = 0;
			while (tmp->args[i] != NULL)
			{
				free(tmp->args[i]);
				i++;
			}
			free(tmp->args);
		}
		free(tmp->arg_quoted);
		free(tmp);
	}
}
 
void	increment_shlvl(char ***env)
{
	char	*shlvl_val;
	int		shlvl_num;
	char	*new_num_str;
	char	*new_shlvl_str;
 
	shlvl_val = get_env_value("SHLVL", *env);
	if (shlvl_val)
		shlvl_num = ft_atoi(shlvl_val) + 1;
	else
		shlvl_num = 1;
	new_num_str = ft_itoa(shlvl_num);
	new_shlvl_str = ft_strjoin("SHLVL=", new_num_str);
	*env = export_remove("SHLVL", *env);
	*env = export_add(new_shlvl_str, *env);
	free(new_num_str);
	free(new_shlvl_str);
}
 
int	check_quotes(char *str)
{
	int	i;
	int	quote;
 
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (quote == 0 && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote == str[i])
			quote = 0;
		i++;
	}
	if (quote != 0)
	{
		print_error(NULL, "syntax error: unclosed quotes", 2);
		return (0);
	}
	return (1);
}
 
static void	print_token_error(t_token *bad_token)
{
	char	*msg;
	char	*full_msg;
 
	if (!bad_token)
		msg = ft_strdup("newline");
	else
		msg = ft_strdup(bad_token->value);
	full_msg = ft_strjoin("syntax error near unexpected token `", msg);
	free(msg);
	msg = ft_strjoin(full_msg, "'");
	free(full_msg);
	print_error(NULL, msg, 2);
	free(msg);
}
 
int	check_syntax(t_token *tokens)
{
	t_token	*tmp;
 
	tmp = tokens;
	if (tokens && tokens->type == PIPE)
	{
		print_token_error(tokens);
		return (0);
	}
	while (tmp)
	{
		if (tmp->type >= REDIR_IN && tmp->type <= APPEND)
		{
			if (!tmp->next || tmp->next->type != WORD)
			{
				print_token_error(tmp->next);
				return (0);
			}
		}
		if (tmp->type == PIPE)
			if (!pipe_error(tmp))
				return (0);
		tmp = tmp->next;
	}
	return (1);
}
