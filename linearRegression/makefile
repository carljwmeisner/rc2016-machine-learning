target: ./a.out

./a.out: ./obj/data.o ./obj/Expansion.o ./obj/LinearRegression.o ./obj/main.o
	g++ -I./include ./obj/data.o ./obj/Expansion.o ./obj/LinearRegression.o ./obj/main.o
./obj/data.o: ./include/data.h ./src/data.cpp
	g++ -c -I./include -o ./obj/data.o ./src/data.cpp
./obj/Expansion.o: ./include/Expansion.h ./include/data.h ./src/Expansion.cpp
	g++ -c -I./include -o ./obj/Expansion.o ./src/Expansion.cpp
./obj/LinearRegression.o: ./include/data.h ./include/LinearRegression.h ./src/LinearRegression.cpp
	g++ -c -I./include -o ./obj/LinearRegression.o ./src/LinearRegression.cpp
./obj/main.o: ./include/data.h ./include/LinearRegression.h ./include/Expansion.h ./src/main.cpp
	g++ -c -I./include -o ./obj/main.o ./src/main.cpp