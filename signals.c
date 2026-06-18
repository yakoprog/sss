#include "minishell.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

/* Extracted inline comments from signals.c:

- history kütüphanesini eklemek güvenlidir
- Ctrl+C (SIGINT) basıldığında çalışacak fonksiyon
- 1. DOKUNUŞ: Bash kuralı! Prompt ekranındayken Ctrl+C basılırsa kod 130 olur.
- 2. Ekrana bir alt satıra inme karakteri bas (Yeni prompt için)
- 3. Readline'a "Hey, artık yeni bir satırdayız, haberin olsun" de
- 4. DOKUNUŞ: Hafızada yarım kalan yazıyı SİL! (Yorum satırından çıkardık)
- 5. Minishell prompt'unu (minishell$ ) alt satıra tekrar çiz!
- Sinyalleri dinlemeye başlama fonksiyonu
- Ctrl+C (SIGINT) gelirse bizim handle_sigint fonksiyonunu çağır
- Ctrl\\ (SIGQUIT) gelirse bunu tamamen YOKSAY (Sadece komut çalışırken işler)

These comments were moved from inside functions to avoid counting as lines inside functions. */

void handle_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}