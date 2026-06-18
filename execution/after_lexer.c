#include "../minishell.h"

/* Extracted inline comments from execution/after_lexer.c:

- Durum 1: Çocuk normal yollarla (exit ile) eceliyle öldüyse
- Durum 2: Çocuk dışarıdan bir sinyalle (Ctrl+C veya Ctrl\\) öldürüldüyse
- WTERMSIG(status) bize sinyal numarasını verir (Ctrl+C -> 2, Ctrl\\ -> 3)
- Bash standardı: 128 + Sinyal Numarası
- Eğer Ctrl\\ (Quit) basıldıysa Bash ekrana "Quit: 3" yazar.
- Ctrl+C basıldığında ekranda alt satıra geçmesi için:
- Geride kalan diğer vagonlar varsa onların da ölmesini bekle
- DİKKAT: Artık char ***env alıyor! (envp kelimesi de env oldu)
- Direkt env gönderiyoruz
- DİKKAT: Artık char ***env alıyor!
- --- ŞEF (PARENT) BYPASS KONTROLÜ ---
- bypass yapıyoruz çünkü tek komut varsa ve o komut bir builtin ise, fork atmaya gerek yok, direkt olarak parent process içinde çalıştırabiliriz, böylece exit status'ü doğru şekilde alabiliriz, çünkü builtin'ler genellikle shell'in kendi durumunu değiştirmek istediği için (örneğin cd, export, unset gibi) fork atmak mantıksız olurdu, bu yüzden bu özel durumu bypass ederek handle ediyoruz
- id ve prev_read_fd'yi adres olarak gönderiyoruz çünkü ft_multiple_pipe içinde değiştireceğiz, böylece sonraki komutlar da bu değişiklikten etkilenmiş olacaklar
- DOKUNUŞ: Döngü bitti, bütün komutlar paralel olarak çalışıyor.
- Şimdi en son komutun (id) bitmesini ve çıkış kodunu ($?) almasını bekle!

These comments were moved from inside functions to avoid counting as lines inside functions. */

static void ft_wait(int id)
{
	int status;

	waitpid(id, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == 3)
			printf("Quit: 3\n");
		else if (WTERMSIG(status) == 2)
			printf("\n");
	}

	while (wait(NULL) > 0)
		;
}

int	ft_bypass(t_cmd *tmp, char ***env)
{
	int	original_out;

	if (tmp != NULL && tmp->next == NULL && is_builtin(tmp->args[0]))
	{
		original_out = dup(1);
		if (tmp->fd_out != 1)
		{
			dup2(tmp->fd_out, 1);
			close(tmp->fd_out);
		}
		execute_builtin(tmp, env);
		dup2(original_out, 1);
		close(original_out);
		return (1);
	}
	return (0);
}

void after_lexer(t_cmd *cmds, char ***env)
{
	t_cmd    *tmp;
	int        id;
	int        prev_read_fd;

	prev_read_fd = 0;
	id = 0;
	tmp = cmds;
	if (ft_bypass(tmp, env))
		return;
	while (tmp != NULL)
	{
		ft_multiple_pipe(tmp, env, &id, &prev_read_fd);
		tmp = tmp->next;
	}
	ft_wait(id);
}
