interpretation : Interpretation.o Camera.o 
	g++ Interpretation.o Camera.o -o interpretation

Interpretation.o : Interpretation.cpp ../protocol_constants.h
	g++ -c -g Interpretation.cpp

Camera.o : Camera.cpp
	g++ -c -g Camera.cpp

clean : 
	rm -f *.o
	rm -f .h.gch

mrproper : clean
	rm -f Interpretation
