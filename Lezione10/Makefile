CC = mpicxx
CFLAGS = -O3 --std=c++14

all: esercizio10.exe esercizio10.simple.exe

esercizio10.exe : system.o esercizio10.o random.o
	$(CC) system.o random.o esercizio10.o -o esercizio10.exe $(CFLAGS)

esercizio10.simple.exe : system.o esercizio10.simple.o random.o
	g++ system.o random.o esercizio10.simple.o -o esercizio10.simple.exe $(CFLAGS)

esercizio10.o : esercizio10.cpp system.h
	$(CC) -c esercizio10.cpp -o esercizio10.o $(CFLAGS)

esercizio10.simple.o : esercizio10.simple.cpp system.h
	g++ -c esercizio10.simple.cpp -o esercizio10.simple.o $(CFLAGS)

system.o : system.cpp system.h random.h
	$(CC) -c system.cpp -o system.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o esercizio10.exe
	rm -f *.o esercizio10.simple.exe

remove :
	rm Costi/*