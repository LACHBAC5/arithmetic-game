
all: main.out

main.out: main.o math-game.o random.o read.o equation.o doomsday.o
	g++ -o $@ $^

%.o: %.cpp
	g++ -o $@ -std=c++20 $< -c
