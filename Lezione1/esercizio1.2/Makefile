CC = g++
CFLAGS = -Wall -O3 --std=c++11

all: esercizio1.2

esercizio1.2: esercizio1.2.o random.o
	$(CC) esercizio1.2.o random.o -o esercizio1.2
esercizio1.2.o: esercizio1.2.cpp
	$(CC) -c esercizio1.2.cpp -o esercizio1.2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o seed.out
