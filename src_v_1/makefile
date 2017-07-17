DIR_SRC = ./src
DIR_TEST = ./tests

SRC = $(wildcard ${DIR_SRC}/*.cpp)

VPATH = ./src
CFLAGS += -I$(DIR_SRC) -std=c++11 -pthread

SRC_OBJ = $(patsubst %.cpp, %.o, ${SRC})

Lib: $(SRC_OBJ)

${DIR_SRC}/%.o:$(DIR_SRC)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

.PHONY: tests clean

tests: 
	for dir in $(DIR_TEST); do\
		(cd $$dir; ${MAKE});\
	done

clean:
	rm -f $(SRC_OBJ)
	for dir in $(DIR_TEST); do\
		(cd $$dir; ${MAKE} clean);\
	done
