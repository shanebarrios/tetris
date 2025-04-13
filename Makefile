SRCS = $(wildcard src/*.cpp)

INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2 -Iinclude
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
COMPILER_FLAGS = -Wall -Wextra -std=c++20
OBJ_NAME = main

all:
	g++ $(SRCS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(COMPILER_FLAGS) -o $(OBJ_NAME)