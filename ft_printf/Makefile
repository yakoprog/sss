NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Werror -Wextra 
RM = rm -f

SRCS = ft_printf.c ft_printf_format.c \
		ft_printf_utils.c ft_printf_utilstwo.c

OBJS = $(SRCS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re