poly.o: poly.cpp poly.h
	g++ -c poly.cpp -Wall -Wextra -Werror -Wshadow -O

main.o: main.cpp poly.h
	g++ -c main.cpp -Wall -Wextra -Werror -Wshadow -O

main: main.o poly.o
	g++ -o main main.o poly.o