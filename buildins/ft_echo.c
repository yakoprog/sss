#include "../minishell.h"

/* Extracted inline comments from buildins/ft_echo.c:

- Eğer ilk argüman "-n" ise, alt satıra inme bayrağını kaldır
- "-n" yazısını atla, kelimeleri basmaya 2. argümandan başla
- Kelimeleri aralarında boşluk bırakarak ekrana bas
- Eğer "-n" bayrağı YOKSA, en son alt satıra in
- Her şey mükemmel çalıştı kodu!

These comments were moved from inside functions to avoid counting as lines inside functions. */

void	ft_echo(t_cmd *cmd)
{
	int i;
	int n_flag;

	n_flag = 0;
	i = 1;
	if (cmd->args[1] != NULL && ft_strncmp(cmd->args[1], "-n", 3) == 0)
	{
		n_flag = 1;
		i++;
	}

	while (cmd->args[i] != NULL)
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1] != NULL)
			printf(" ");
		i++;
	}

	if (n_flag == 0)
		printf("\n");
	g_exit_status = 0;
}
