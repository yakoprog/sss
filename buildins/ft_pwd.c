#include "../minishell.h"

/* Extracted inline comments from buildins/ft_pwd.c:

- PWD komutunun asıl amacı ekrana çıktı vermektir,
- bu yüzden burada printf() GÜVENLİDİR ve KULLANILMALIDIR.
- Başarı kodu
- İşletim sistemi konumu bulamazsa hata bas

These comments were moved from inside functions to avoid counting as lines inside functions. */

void	ft_pwd(void)
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		g_exit_status = 0;
	}
	else
	{
		print_error("pwd", strerror(errno), 1);
	}
}
