NAME := fractal
CC := c++
FLAGS := -Wall -Wextra -Werror
SDL_FLAGS := $(shell sdl2-config --cflags --libs)
SRCS := main.cpp \
		fractal_calc.cpp
OBJS := ${SRCS:.cpp=.o}

.cpp.o:
	${CC} ${FLAGS} $(shell sdl2-config --cflags) -c $< -o ${<:.cpp=.o}

CLR_RMV := \033[0m
RED := \033[1;31m
GREEN := \033[1;32m
YELLOW := \033[1;33m
BLUE := \033[1;34m
CYAN := \033[1;36m
RM := rm -f

${NAME}: ${OBJS}
	@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
	${CC} ${FLAGS} -o ${NAME} ${OBJS} $(SDL_FLAGS)
	@echo "$(GREEN)$(NAME) created[0m ✔️"

all: ${NAME}

bonus: all

clean:
	@ ${RM} *.o */*.o */*/*.o
	@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean: clean
	@ ${RM} ${NAME}
	@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

re: fclean all

.PHONY: all clean fclean re