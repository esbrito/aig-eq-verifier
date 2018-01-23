main: 
	g++ -std=c++11 -o myprogram aagReader.cpp aig.cpp aigAPI.cpp main.cpp gerentebdd.cpp


clean: 
	rm -rf *.o myprogram