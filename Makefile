#==============================================================================#
#                                   TARGETS                                    #
#==============================================================================#

NAME = ircserv

#==============================================================================#
#                                   COMMANDS                                   #
#==============================================================================#

CXX = c++ 
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
DFLAGS = -MMD
MKDIR = mkdir -p
RM = rm -f
MAKE_SILENT = make --no-print-directory

#==============================================================================#
#                                    COLORS                                    #
#==============================================================================#

PURPLE = \033[35m
GREEN = \033[32m
YELLOW = \033[33m
END = \033[0m

#==============================================================================#
#                                    PATHS                                     #
#==============================================================================#

SRC_DIR = src/
HEADER_DIR = include/
OBJ_DIR = obj/

#==============================================================================#
#                                   SOURCES                                    #
#==============================================================================#

SRC = main.cpp serveur.cpp utilsServeur.cpp client.cpp channel.cpp auth.cpp utils.cpp prefixe.cpp handleCommand.cpp handleMode.cpp handleJoin.cpp handleKick.cpp handleQuit.cpp handleInvite.cpp handleTopic.cpp handleNotice.cpp handlePart.cpp handlePrivmsg.cpp handleKill.cpp
#==============================================================================#
#                                   HEADERS                                    #
#==============================================================================#

HEAD = $(addprefix $(HEADER_DIR), Serveur.hpp Client.hpp Responses.hpp Channel.hpp) \

#==============================================================================#
#                                   OBJECTS                                    #
#==============================================================================#

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))

#==============================================================================#
#                                   MAKEFILE                                   #
#==============================================================================#

all : $(OBJ_DIR) $(NAME)

$(NAME) : $(OBJ)
	echo "$(YELLOW)Making IRC Exercise$(END)"
	$(CXX) $(OBJ) -o $(NAME)
	echo "$(GREEN)Done$(END)"

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(HEAD) Makefile
	$(CXX) $(CXXFLAGS) $(DFLAGS) -c $< -o $@

$(OBJ_DIR) :
	$(MKDIR) $(OBJ_DIR)

clean :
	echo "$(PURPLE)Cleaning IRC objects...$(END)"
	$(RM)r $(OBJ_DIR)
	echo "$(GREEN)Done$(END)"

fclean : clean
	echo "$(PURPLE)Cleaning IRC...$(END)"
	$(RM) $(NAME)
	echo "$(GREEN)Done$(END)"

re : fclean
	$(MAKE_SILENT) all

.PHONY : all clean fclean re
.SILENT :