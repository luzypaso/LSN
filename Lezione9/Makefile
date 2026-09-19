CC = g++
CFLAGS = -O3 --std=c++14
all: esercizio9.exe
esercizio9.exe : system.o esercizio9.o random.o
	$(CC) system.o random.o esercizio9.o -o esercizio9.exe $(CFLAGS)
esercizio9.o : esercizio9.cpp system.h
	$(CC) -c esercizio9.cpp -o esercizio9.o $(CFLAGS)
system.o : system.cpp system.h random.h
	$(CC) -c system.cpp -o system.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o esercizio9.exe
