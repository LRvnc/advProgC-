CLIENT = client.o
SERVER = server.o

all: client server

server: $(SERVER)
	c++ -o server $(SERVER) -lpthread

client: $(CLIENT)
	c++ -o client $(CLIENT) -lpthread

-include $(CLIENT:.o=.d)
-include $(SERVER:.o=.d)

%.o: %.cpp
	c++ -c $*.cpp -o $*.o -Wall -Wshadow -O
	c++ -MM $*.cpp > $*.d