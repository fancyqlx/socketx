DIR_SRC = ./src
DIR_EXP = ./examples/echo ./examples/multi-echo

SRC = $(wildcard ${DIR_SRC}/*.cpp)

VPATH = ./src
CFLAGS += -I$(DIR_SRC) -std=c++11 -pthread

SRC_OBJ = $(patsubst %.cpp, %.o, ${SRC})

Lib: $(SRC_OBJ)

${DIR_SRC}/%.o:$(DIR_SRC)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

.PHONY: examples clean

examples: 
	for dir in $(DIR_EXP); do\
		(cd $$dir; ${MAKE});\
	done

clean:
	rm -f $(SRC_OBJ)
	for dir in $(DIR_EXP); do\
		(cd $$dir; ${MAKE} clean);\
	done
