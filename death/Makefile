
SRC_MAIN		= src/famine.asm
SRC_COMPRESS	= src/compress.asm
SRC_LZ4K		= src/lz4k.c
SRC_RC6			= src/rc6.c

SOURCE_FASMG	= fasmg-master

INCLUDE_ASM		=	include/macro/struct.inc\
					include/macro/instruction.inc\
					include/macro/proc64.inc\
					include/linux/ptrace.inc\
					include/linux/fcntl.inc\
					include/linux/mman.inc\
					include/linux/elf.inc\
					include/linux/wait.inc\
					include/linux/sigaction.inc\
					include/main.inc\
					include/rc6.inc\
					include/linux/unistd_64.inc

FASMG = $(abspath $(SOURCE_FASMG)/core/fasmg.x64)

# Делаем нашу фактическую цель - финальный файл
NAME_MAIN		= Death
NAME_LZ4K		= $(basename $(notdir $(SRC_LZ4K)))
NAME_RC6		= $(basename $(notdir $(SRC_RC6)))
NAME_COMPRESS	= $(basename $(notdir $(SRC_COMPRESS)))

# Самое первое правило которое действует по умолчаниюю
all: $(NAME_MAIN)

# Удаляем все обьектники кроме основного
clean:
	@rm -f tmp.txt
	@rm -f $(NAME_RC6)
	@rm -f $(NAME_LZ4K)
	@rm -f $(NAME_COMPRESS).lz4k
	@rm -fdr $(SOURCE_FASMG)

# Все удаляем
fclean: clean
	@rm -f $(NAME_MAIN)

# Удалим все обектники и скомпилим заново
re: fclean all

# Правило срабатывающее при проверки цели основной - что переводит на проверку папок для обьектников и потом действительного финальной цели
$(NAME_MAIN): $(SOURCE_FASMG) $(NAME_LZ4K) $(NAME_RC6) $(NAME_COMPRESS).lz4k $(SRC_MAIN) $(INCLUDE_ASM)
	@INCLUDE="$(SOURCE_FASMG)/core/examples/x86/include/;$(SOURCE_FASMG)/packages/;./" $(FASMG) $(SRC_MAIN) $(NAME_MAIN) > tmp.txt
	@./$(NAME_RC6) $(NAME_MAIN) 0<tmp.txt
	@cat tmp.txt
	@rm -f tmp.txt
	@chmod 777 $(NAME_MAIN)

$(NAME_COMPRESS).lz4k: $(SRC_COMPRESS) $(INCLUDE_ASM)
	@INCLUDE="$(SOURCE_FASMG)/core/examples/x86/include/;$(SOURCE_FASMG)/packages/;./" $(FASMG)  $(SRC_COMPRESS) $(NAME_COMPRESS)
	./$(NAME_LZ4K) -c -m soled $(NAME_COMPRESS)
	@rm -f $(NAME_COMPRESS)

$(NAME_LZ4K): $(SRC_LZ4K)
	gcc -Wall -Wextra -Werror -m64 -O2 $(SRC_LZ4K) -o $(NAME_LZ4K) -I include/
	@chmod 777 $(NAME_LZ4K)

$(NAME_RC6): $(SRC_RC6)
	gcc -Wall -Wextra -Werror -m64 -O2 $(SRC_RC6) -o $(NAME_RC6) -I include/
	@chmod 777 $(NAME_RC6)

$(SOURCE_FASMG):
	@git clone https://github.com/tgrysztar/fasmg.git $(SOURCE_FASMG)
	@chmod 777 $(SOURCE_FASMG)/core/fasmg.x64

# Список используемых команд в MakeFile что бы не спутало с файлами в каталоге
.PHONY: all clean fclean re
