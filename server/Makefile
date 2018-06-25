server:SocketMatTransmissionServer.o SocketServerMat.o
	g++ SocketMatTransmissionServer.cpp SocketServerMat.cpp `pkg-config --cflags --libs opencv` -o server
SocketServerMat.o:SocketServerMat.cpp
	g++ SocketServerMat.cpp `pkg-config --cflags --libs opencv` -c -o SocketServerMat.o
SocketMatTransmissionServer.o:SocketMatTransmissionServer.cpp SocketMatTransmissionServer.h
	g++ SocketMatTransmissionServer.cpp `pkg-config --cflags --libs opencv` -c -o SocketMatTransmissionServer.o
clean:
	rm *.o
