/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycinarog <ycinarog@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 14:37:15 by ycinarog          #+#    #+#             */
/*   Updated: 2026/06/18 14:37:15 by ycinarog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

static void	setup_shell(char **envp, char ***my_env)
{
	*my_env = copy_env(envp);
	increment_shlvl(my_env);
	init_signals();
}

static void	process_command_line(char *input, char ***my_env)
{
	t_token	*token_list;
	t_cmd	*cmd_list;

	if (!check_quotes(input))
		return ;
	token_list = NULL;
	cmd_list = NULL;
	lexer(input, &token_list);
	if (!check_syntax(token_list))
	{
		free_tokens(token_list);
		return ;
	}
	parse_tokens(token_list, &cmd_list);
	expand_cmds(cmd_list, *my_env);
	after_lexer(cmd_list, my_env);
	free_tokens(token_list);
	free_cmds(cmd_list);
}

static void	shell_loop(char ***my_env)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			shell->exit_status = 130;
			g_signal = 0;
		}
		if (input == NULL || ft_strncmp(input, "exit", 5) == 0)
		{
			ft_putstr_fd("exit\n", 2);
			if (input != NULL)
				free(input);
			break ;
		}
		if (*input)
		{
			add_history(input);
			process_command_line(input, my_env);
		}
		free(input);
	}
}

static void	cleanup_shell(char **my_env)
{
	ft_free_split(my_env);
	clear_history();
}

int	main(int ac, char **av, char **envp)
{
	char	**my_env;

	(void)av;
	if (ac != 1)
	{
		print_error(NULL, "Minishell does not take arguments", 1);
		return (1);
	}
	setup_shell(envp, &my_env);
	shell_loop(&my_env);
	cleanup_shell(my_env);
	return (g_exit_status);
}
