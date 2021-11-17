# Имя выходного файла и проверка на расширение для Windows
NAME = ft_ls

LIST		=	main.c\
				ft_ls_main.c\
				ft_ls_list_add_invalid.c\
				ft_ls_list_add_file.c\
				ft_ls_list_add_dir.c\
				ft_ls_display.c\
				ft_ls_display_full.c\
				ft_ls_display_full_add.c\
				ft_ls_write.c\
				ft_ls_sort_strcmp.c\
				ft_ls_sort_strcmp_add.c\
				ft_ls_recursive.c\
				ft_ls_recursive_add.c\
				ft_ls_free.c

# Все деректории в которых будим искать си файлы для компиляции
DIRS	   = src/
# Получаем Список обькектных файлов которые должны быть или иначе создаем
SOURCE		= $(addprefix $(DIRS), $(LIST))
OBJ			= $(patsubst %.c,%.o,$(SOURCE))

# Список дериктории для библиотек
LIB_DIRS        = libft/
# Список файлов с биоблитек которые подключаем при компиляции
LIB 	   		= libft/libft.a

# Для удобства и универсальности что бы обработать биоблитеки с помощью ихнего MakeFile
LIB_CMD_ALL     = $(addsuffix .all,$(LIB_DIRS))
LIB_CMD_DEBUG   = $(addsuffix .debug,$(LIB_DIRS))
LIB_CMD_TEST    = $(addsuffix .test,$(LIB_DIRS))
LIB_CMD_CLEAN   = $(addsuffix .clean,$(LIB_DIRS))
LIB_CMD_FCLEAN  = $(addsuffix .fclean,$(LIB_DIRS))

# Список папок биоблитек с которых берем хедары
INCLUDES		= includes/
INCLUDES_LIBFT	= $(addsuffix includes/,$(LIB_DIRS))

# Флаги компиляции по умолчанию
override OPFLAGS ?= -O2
override FLAGS ?= -Wall -Wextra -Werror

# Самое первое правило которое действует по умолчанию
#all: $(LIB_CMD_ALL) $(NAME)
all:	$(NAME)

# Команда которая компирует отладочную версию
debug: $(LIB_CMD_DEBUG)
	make OPFLAGS="-O0 -g"

# Команда которая компирует не вызывая минимум сообщений об ошибках
test: $(LIB_CMD_TEST)
	make FLAGS=""

# Удалим все обьктные файлы созданые нами
clean: $(LIB_CMD_CLEAN)
	rm -f $(OBJ) $(wildcard $(addsuffix *.d,$(DIRS)))

# Удалим финальный файл
fclean: $(LIB_CMD_FCLEAN) clean
	rm -f $(NAME)

# Очистим все и скомпилируем заново
re: fclean all

# Правило которое если нету обьктного файла соответсвуещего сишному файлу или сищный файл изменился тогда скомпилируем обьктный файл
%.o: %.c
	gcc $(FLAGS) $(OPFLAGS) -c $< -o $@ -I$(INCLUDES_LIBFT) -I$(INCLUDES) -MD

# Если обьктынй файл или биоблитека изменилась занова перекомпилируем выходной файл
$(NAME): $(LIB_CMD_ALL) $(OBJ) $(LIB)
	gcc $(FLAGS) $(OPFLAGS) $(OBJ) -o $(basename $(NAME)) $(LIB)

# Правило которое соответсвует $(LIB_CMD_ALL)
%.all:
	make -C $*

# Правило которое соответсвует $(LIB_CMD_DEBUG) ......
%.debug:
	make -C $* OPFLAGS="-O0 -g"

%.test:
	make -C $* FLAGS=""

%.clean:
	make -C $* clean

%.fclean:
	rm -f $(addsuffix *.a,$*)

# При компеляции обьктых файлов создаються файлы соответсвий - тоесть что требеуться для компеляции данного сишного файла в обьктынй и при измененении одного из них заного скомпилируеться
include $(wildcard $(addsuffix *.d,$(DIRS)))

# Список используемых команд в MakeFile что бы не спутало с файлами в каталоге
.PHONY: all clean fclean re debug test
