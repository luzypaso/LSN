CC = g++
CFLAGS = -Wall -O3 --std=c++11

esercizio8.1 : esercizio8.1.o random.o funzioni.o
	$(CC) random.o funzioni.o esercizio8.1.o -o esercizio8.1
esercizio8.1.o : esercizio8.1.cpp
	$(CC) -c esercizio8.1.cpp -o esercizio8.1.o $(CFLAGS)
funzioni.o: funzioni.cpp funzioni.h
	$(CC) -c funzioni.cpp -o funzioni.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o esercizio8.1 seed.out