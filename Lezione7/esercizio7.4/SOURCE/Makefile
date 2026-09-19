CC = g++
CXXFLAGS = -O3 -std=c++14 -fPIE
AFLAGS = -larmadillo

all: simulator.exe

simulator.exe : system.o NSL_SIMULATOR.o particle.o random.o
	$(CC) system.o particle.o random.o NSL_SIMULATOR.o -o simulator.exe $(AFLAGS)

NSL_SIMULATOR.o : NSL_SIMULATOR.cpp system.h particle.h
	$(CC) -c NSL_SIMULATOR.cpp -o NSL_SIMULATOR.o $(CXXFLAGS)

system.o : system.cpp system.h random.h particle.h
	$(CC) -c system.cpp -o system.o $(CXXFLAGS)

particle.o : particle.cpp particle.h
	$(CC) -c particle.cpp -o particle.o $(CXXFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CXXFLAGS)

clean :
	rm -f *.o simulator.exe

remove:
	rm -f ../OUTPUT/*.*
	rm -f ../OUTPUT/CONFIG/*.*