#include "minishell.h"

// 42 KURALI: Sinyaller ve çıkış kodları ($?) için tek bir global değişkene izin var!
int g_exit_status = 0;

/*
Notes moved from inside `main` (comments were inside the function and counted as lines):

- `input` sadece `readline` ile alınan komut satırını tutacak, `token_list` lexer'ın ürettiği tokenları tutacak, `cmd_list` ise `parse_tokens`'un ürettiği komut listesini tutacak.
- 1. DOKUNUŞ: Argüman kontrolü ve hata bastırma (program `./minishell arguman` ile çalıştırılırsa reddedilsin).
- Program başlar başlamaz sistemin çevresini kopyala (`my_env = copy_env(envp)`).
- 2. DOKUNUŞ: `SHLVL`'yi 1 artır; yeni bir kabuk açıldığını belirtmek için gerekli.
- Sinyalleri dinle (Ctrl+C, Ctrl+\) ve `g_exit_status` globalini sinyal işleyicisinde kullanma gerekçesi.
- Döngü içinde: prompt, Ctrl+D (EOF) ve `exit` kontrolü; `add_history` kullanımı.
- 1. KAPI: Tırnak bekçisi (`check_quotes`).
- 2. KAPI: Syntax (dilbilgisi) bekçisi (`check_syntax`).
- `lexer`, `parse_tokens`, `expand_cmds`, `after_lexer` akışı: lexer -> parser -> expander -> executor.
- Bellek temizliği: `free_tokens`, `free_cmds`, `free(input)` her döngü sonrası ve program sonu için `free_env`, `clear_history`.
- 3. DOKUNUŞ: İşletim sistemine her zaman "0" (başarılı) dön.

These comments were moved outside the function to avoid counting them as lines inside `main`.
*/

static void setup_shell(char **envp, char ***my_env)
{
	*my_env = copy_env(envp);
	increment_shlvl(my_env);
	init_signals();
}

static void process_command_line(char *input, char **my_env)
{
	t_token *token_list;
	t_cmd *cmd_list;

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
	expand_cmds(cmd_list, my_env);
	after_lexer(cmd_list, &my_env);
	free_tokens(token_list);
	free_cmds(cmd_list);
}

static void shell_loop(char **my_env)
{
	char *input;

	while (1)
	{
		input = readline("minishell$ ");
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

static void cleanup_shell(char **my_env)
{
	free_env(my_env);
	clear_history();
}

int main(int ac, char **av, char **envp)
{
	char **my_env;

	(void)av;
	if (ac != 1)
	{
		print_error(NULL, "Minishell does not take arguments", 1);
		return (1);
	}
	setup_shell(envp, &my_env);
	shell_loop(my_env);
	cleanup_shell(my_env);
	return (g_exit_status);
}
