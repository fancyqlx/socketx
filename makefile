DIR_SRC = ./src
DIR_EXP = ./examples

SRC = $(wildcard ${DIR_SRC}/*.cpp)
EXP = $(wildcard ${DIR_EXP}/*.cpp)

VPATH = ./src:./examples
CFLAGS += -I$(DIR_SRC) -std=c++11

SRC_OBJ = $(patsubst %.cpp, %.o, ${SRC})
EXP_OBJ = $(patsubst %.cpp, %.o, ${EXP})

Lib: $(SRC_OBJ)

${DIR_SRC}/%.o:$(DIR_SRC)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

.PHONY: examples clean

examples: echo_client echo_server

echo_client: $(DIR_EXP)/echo_client.o ${SRC_OBJ}
	g++ $(CFLAGS) $(DIR_EXP)/echo_client.o ${SRC_OBJ} -o $(DIR_EXP)/echo_client

echo_server: $(DIR_EXP)/echo_server.o ${SRC_OBJ}
	g++ $(CFLAGS) $(DIR_EXP)/echo_server.o ${SRC_OBJ} -o $(DIR_EXP)/echo_server

$(DIR_EXP)/%.o:$(DIR_EXP)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm $(SRC_OBJ) $(EXP_OBJ)
