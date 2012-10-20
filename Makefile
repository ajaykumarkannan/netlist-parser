CFLAGS = -Wall
OBJ = ./obj
SRC = ./src
BIN = ./bin
INC = ./include

all: graph.o
	g++ $(CFLAGS) $(OBJ)/* $(SRC)/main.cpp -fopenmp -o $(BIN)/main.out

graph.o:
	g++ $(CFLAGS) $(SRC)/graph.cpp -c -o $(OBJ)/graph.o

clean:
	rm $(OBJ)/* $(BIN)/*
