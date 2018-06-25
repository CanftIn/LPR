CC=g++
CFLAG=`pkg-config --cflags --libs opencv` -w
arm_client:main.o common.o HSVpreprocess.o RGBPreprocess.o create_lib.o
	$(CC) main.o common.o HSVpreprocess.o RGBPreprocess.o create_lib.o $(CFLAG) -o arm_client
main.o:main.cpp
	$(CC) main.cpp $(CFLAG) -c -o main.o
common.o:common.cpp common.h
	$(CC) common.cpp $(CFLAG) -c -o common.o
create_lib.o:create_lib.cpp create_lib.h 
	$(CC) create_lib.cpp $(CFLAG) -c -o create_lib.o
HSVpreprocess.o:HSVpreprocess.cpp HSVpreprocess.h
	$(CC) HSVpreprocess.cpp $(CFLAG) -c -o HSVpreprocess.o
RGBPreprocess.o:RGBPreprocess.cpp RGBPreprocess.h 
	$(CC) RGBPreprocess.cpp $(CFLAG) -c -o RGBPreprocess.o
clean:
	rm *.o             
