NAME		= libft.a
DIRS		= srcs/
DIRS_D		= srcs/ srcs/ft_printf/
LIST		=	ft_atoi.c\
				ft_gmtime_r.c\
				ft_itoa.c\
				ft_memset.c\
				ft_printf.c\
				ft_snprintf.c\
				ft_printf/fn_big.c\
				ft_printf/fn_display_a.c\
				ft_printf/fn_display_b.c\
				ft_printf/fn_display_c.c\
				ft_printf/fn_display_d.c\
				ft_printf/fn_display_e.c\
				ft_printf/fn_display_f.c\
				ft_printf/fn_display_float.c\
				ft_printf/fn_display_k.c\
				ft_printf/fn_display_n.c\
				ft_printf/fn_display_o.c\
				ft_printf/fn_display_p.c\
				ft_printf/fn_display_s.c\
				ft_printf/fn_display_u.c\
				ft_printf/fn_display_unknown.c\
				ft_printf/fn_display_x.c\
				ft_printf/fn_display_r.c\
				ft_printf/fn_display.c\
				ft_printf/fn_sp.c\
				ft_printf/fn_str_copy.c\
				ft_printf/fn_str_time.c\
				ft_printf/fn_write_float.c\
				ft_strchr.c\
				ft_strdup.c\
				ft_strndup.c\
				ft_strcmp.c\
				ft_strlen.c\
				ft_wcslen.c\
				ft_wctomb.c
SOURCE		= $(addprefix $(DIRS), $(LIST))
OBJ			= $(patsubst %.c,%.o,$(SOURCE))
INCLUDES	= includes/

override OPFLAGS ?= -O2
override FLAGS ?= -Wall -Wextra -Werror

all: $(OBJ) $(NAME)

debug:
	make OPFLAGS="-O0 -g"

test:
	make FLAGS=""

%.o: %.c
	gcc $(FLAGS) $(OPFLAGS) -I$(INCLUDES) -c $< -o $@ -MD

$(NAME): $(OBJ)
	ar rc $(NAME) $?
	ranlib $(NAME)

clean:
	rm -f $(OBJ) $(patsubst %.c,%.d,$(SOURCE))

fclean: clean
	rm -f $(NAME)

re: fclean all

include $(wildcard $(addsuffix *.d,$(DIRS_D)))

.PHONY: all clean fclean re debug test
