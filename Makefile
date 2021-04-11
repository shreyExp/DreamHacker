main: main.cpp
	g++ -g -o main main.cpp -lwiringPi -lm -lpthread -lcurl -lfcgi
