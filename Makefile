CXX = g++
CXXFLAGS = -Wall -std=c++17

OBJS = main.o dma.o uart.o spi.o

all: main

main: $(OBJS)
	$(CXX) -o main $(OBJS)

clean:
	rm -f *.o main
