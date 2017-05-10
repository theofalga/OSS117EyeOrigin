protocol : Protocol.o 
	g++ `pkg-config opencv --cflags` Protocol.o -o protocol `pkg-config opencv --libs`

Protocol.o : Protocol.cpp ../protocol_constants.h
	g++ -c -g Protocol.cpp

clean : 
	rm *.o
	rm *.h.gch

mrproper : clean
	rm -f Protocol
