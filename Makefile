all: processor constructor feature

processor: DataProcessor.cpp
	g++ DataProcessor.cpp -o processor

constructor: DataConstructor.cpp
	g++ DataConstructor.cpp -o constructor

feature: main.o feature.o
	g++ main.o feature.o `pkg-config --cflags --libs gsl` -o feature
	export LD\_LIBRARY\_PATH=/usr/local/lib
main.o: main.cpp
	g++ -c main.cpp

feature.o: feature.cpp feature.h
	g++ -c feature.cpp `pkg-config --cflags --libs gsl`

clean:
	rm *.o main processor constructor feature
