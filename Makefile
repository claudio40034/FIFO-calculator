all: mainServer mainClient
mainServer: mainServer.o Queue.o Calculator.o
	g++ -o mainServer mainServer.cpp Queue.cpp Calculator.cpp

mainClient: mainClient.o Queue.o
	g++ -o mainClient mainClient.cpp Queue.cpp

mainServer.o: mainServer.cpp
	g++ -c mainServer.cpp

mainClient.o: mainClient.cpp
	g++ -c mainClient.cpp

Queue.o: Queue.h Queue.cpp
	g++ -c Queue.cpp

Calculator.o: Calculator.h Calculator.cpp 
	g++ -c Calculator.cpp
clean:
	rm *.o mainServer mainClient



