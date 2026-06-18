#include "../minishell.h"

/* Extracted inline comments from buildins/ft_env.c:

- Yine asıl amacı veri bastırmak olduğu için printf() doğrudur.
- Başarı kodu

These comments were moved from inside functions to avoid counting as lines inside functions. */

void	ft_env(char ***env)
{
	int i;

	i = 0;
	while ((*env)[i] != NULL)
	{
		printf("%s\n", (*env)[i]);
		i++;
	}
	g_exit_status = 0;
}
