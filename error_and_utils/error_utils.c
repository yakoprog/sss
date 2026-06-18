#include "../minishell.h"

/* Extracted inline comments from error_and_utils/error_utils.c:

- Hem stderr'e hata basar, hem de global çıkış kodunu günceller
- export veya unset içinde geçersiz bir isim (örn: 1degisken) yakalarsan:
- "export" veya "unset"
- "1YENI"
- Bash kuralı: İlk harf rakam olamaz. Sadece harf, rakam ve '_' içerebilir.
- 1. İlk karakter harf (a-z, A-Z) veya alt çizgi (_) olmalı
- Yanlışsa 0 dön
- 2. Eşittir ('=') işaretine veya string'in sonuna kadar kontrol et
- Aradaki karakterler sadece harf, rakam veya alt çizgi olabilir
- Kurallara uyuyorsa 1 dön

These comments were moved from inside functions to avoid counting as lines inside functions. */

void	print_error(char *cmd, char *msg, int err_code)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
    
	g_exit_status = err_code;
}

void	export_error(char *cmd_name, char *bad_arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(bad_arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);

	g_exit_status = 1;
}

int	is_valid_env_name(char *str)
{
	int i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
int	pipe_error(t_token *tokens)
{
	t_token *tmp;

	tmp = tokens;
	if (!tmp->next || tmp->next->type == PIPE)
	{
		print_error(NULL, "syntax error near unexpected token `|'", 258);
		return (0);
	}
	return (1);
}
