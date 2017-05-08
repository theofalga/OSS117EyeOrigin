eyelike : Main.o findEyeCenter.o helpers.o 
	g++ `pkg-config opencv --cflags` Main.o findEyeCenter.o helpers.o  -o eyelike `pkg-config opencv --libs`

Main.o : Main.cpp constants.h findEyeCenter.h ../protocol_constants.h
	g++ -c -g Main.cpp

findEyeCenter.o : findEyeCenter.cpp constants.h helpers.h
	g++ -c -g findEyeCenter.cpp

helpers.o : helpers.cpp constants.h
	g++ -c -g helpers.cpp

clean : 
	rm -f *.o
	rm -f .h.gch

mrproper : clean
	rm -f Main
