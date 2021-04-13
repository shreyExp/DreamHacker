main: main.cpp PulseSensor.h
	g++ -g -o main main.cpp CppTimer.cpp -lwiringPi -lm -lpthread -lcurl -lfcgi -lrt 
