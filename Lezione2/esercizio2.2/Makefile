CC = g++
CFLAGS = -Wall -O3 --std=c++11

all: esercizio2.2
esercizio2.2: esercizio2.2.o random.o
	$(CC) esercizio2.2.o random.o -o esercizio2.2
esercizio2.2.o: esercizio2.2.cpp
	$(CC) -c esercizio2.2.cpp -o esercizio2.2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o seed.out
