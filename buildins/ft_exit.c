#include "../minishell.h"

/* Extracted inline comments from buildins/ft_exit.c:

- Exit'in yanına yazılan şey tamamen sayılardan mı oluşuyor kontrolü
- Sadece "-" veya "+" yazıldıysa
- Bash exit komutunu her zaman Standart Hataya (2) yazar
- Durum 1: Sadece "exit" yazıldıysa (O anki çıkış koduyla kapan)
- Durum 2: Harf girildiyse (örn: exit sjsjs) -> Program 255 ile KAPANIR
- Özel hata mesajı (Bash formatı: minishell: exit: sjsjs: numeric argument required)
- Durum 3: Çok fazla argüman varsa (örn: exit 1 2 3) -> Program KAPANMAZ!
- DİKKAT: Exit yapmıyoruz, return ile shell'e dönüyoruz!
- Durum 4: Geçerli bir sayı girildiyse (örn: exit 42) -> O sayıyla kapan

These comments were moved from inside functions to avoid counting as lines inside functions. */

static int	is_numeric(char *str)
{
	int i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_cmd *cmd)
{
	ft_putstr_fd("exit\n", 2);

	if (cmd->args[1] == NULL)
		exit(g_exit_status);

	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(255);
	}

	if (cmd->args[2] != NULL)
	{
		print_error("exit", "too many arguments", 1);
		return ;
	}

	exit(ft_atoi(cmd->args[1]) % 256);
}
