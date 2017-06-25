DIR_SRC = ../../src

SRC = $(wildcard ${DIR_SRC}/*.cpp)

VPATH = $(DIR_SRC)
CFLAGS += -I$(DIR_SRC) -std=c++11 -pthread

SRC_OBJ = $(patsubst %.cpp, %.o, ${SRC})

OBJ = echo_client.o echo_server.o 

multi-echo: echo_client echo_server
	
echo_client: echo_client.o ${SRC_OBJ}
	g++ $(CFLAGS) echo_client.o ${SRC_OBJ} -o echo_client

echo_server: echo_server.o ${SRC_OBJ}
	g++ $(CFLAGS) echo_server.o ${SRC_OBJ} -o echo_server

./%.o:./%.cpp
	g++ $(CFLAGS) -c $< -o $@

${DIR_SRC}/%.o:$(DIR_SRC)/%.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_OBJ) ./*.o echo_client echo_server