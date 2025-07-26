.PHONY:     			all $(NAME) clear mkbuild lib minilibx clean fclean re norm

RED			            = \033[1;31m
GREEN					= \033[1;32m
PURPLE					= \033[1;35m
BLUE			        = \033[0;36m
END						= \033[0m

NAME					= lem-in

BUILD_DIR				= build/
	
HEADER_DIR				= header/
HEADER_FILE				= lem_in.h

DIR						= src/
SRC			 			= main.c \
							utils.c\
							room.c\
							link.c\
							free.c\
							graphics/window.c\
							graphics/key_handler.c\
							graphics/camera.c\
							graphics/hud.c\
							display_data.c\
							queue.c\
							\
							parsing/get_data.c\
							parsing/parsing_utils.c\
							parsing/parse_data.c\
							parsing/parse_room.c\
							parsing/parse_link.c\
							parsing/parse_ant.c\
							\
							errors/print_error.c\
							errors/fatal_errors_handler.c\
							\
							ants/init_ants.c\
							ants/move_ants.c\
							\
							algo/algo_manager.c\

							
OBJECTS			    	= $(SRC:%.c=$(BUILD_DIR)%.o)
	
LIBFT					= libft.a
LIB_DIR					= libft/

GCC						= gcc
CFLAGS					= -Wall -Wextra -Werror -I/usr/include -O3 -c

LDFLAGS					= -L/usr/lib -lGLEW -lGL -lGLU -lglut -lm

RM 						= rm -rf
CLEAR					= clear

$(BUILD_DIR)%.o:		$(DIR)%.c $(LIB_DIR) $(HEADER_DIR)/$(HEADER_FILE)
							@mkdir -p $(@D)
							$(GCC) $(CFLAGS) -I$(HEADER_DIR) -I$(LIB_DIR) $< -o $@ -g

all: 					clear mkbuild lib $(HEADER_DIR) $(NAME) 
							@echo "$(GREEN)[LEM-IN IS READY] $(END)"
							
mkbuild:
							@mkdir -p build

clear:
							$(CLEAR)
						
$(NAME): 				$(OBJECTS) $(LIB_DIR)$(LIBFT) libft
							@$(GCC) $(OBJECTS) -o $(NAME) $(LIB_DIR)$(LIBFT) $(LDFLAGS)
							
lib:
							@make -C $(LIB_DIR)
							@echo "$(GREEN)[LIBFT OK] $(END)"
						
clean:					
							@${RM} $(OBJECTS)
							@make clean -C $(LIB_DIR)  
							@${RM} $(BUILD_DIR)
							@echo "$(RED)[CLEAN DONE]$(END)"

fclean:					clean
							@${RM} ${NAME}
							@make fclean -C $(LIB_DIR) 
							@echo "$(RED)[FCLEAN DONE]$(END)"

norm:
							@norminette $(DIR)

sanit:					$(OBJECTS) $(LIB_DIR)$(LIBFT) libft
							@$(GCC) $(OBJECTS) -o $(NAME) $(LIB_DIR)$(LIBFT) $(LDFLAGS) -fsanitize=address

re:						fclean all
							$(MAKE) all
							@echo "$(GREEN)[REMAKE DONE] $(END)"
