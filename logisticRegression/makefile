target: all

all: ./obj/data.o ./obj/Expansion.o ./obj/LinearRegression.o ./obj/main.o
	g++ -I./include ./obj/data.o ./obj/Expansion.o ./obj/LinearRegression.o ./obj/main.o
./obj/data.o: ./src/data.cpp ./include/data.h
	g++ -o ./obj/data.o -c -I./include ./src/data.cpp
./obj/Expansion.o: ./src/Expansion.cpp ./include/data.h ./include/Expansion.h
	g++ -o ./obj/Expansion.o -c -I./include ./src/Expansion.cpp
./obj/LinearRegression.o: ./src/LinearRegression.cpp ./include/data.h
	g++ -o ./obj/LinearRegression.o -c -I./include ./src/LinearRegression.cpp
./obj/main.o: ./src/main.cpp ./include/data.h ./include/LinearRegression.h ./include/Expansion.h
	g++ -o ./obj/main.o -c -I./include ./src/main.cpp

