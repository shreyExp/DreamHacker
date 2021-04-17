#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <QApplication>
#include "mainwindow.h"
#include "CppTimer.h"
#include <thread>
#include "PulseSensor.h"




volatile int g_running = 1;

void signalHandler(int signum){
	g_running = 0;
	printf("Program Terminated\n");
}




int main(int argc, char *argv[])
{
	int mode = 0;
	if(argc > 1){
		mode = atoi(argv[1]);
	}
	signal(SIGINT, signalHandler);
	/**
	 * SensorTimer runs in a thread
	 * It reads the analog data from pulse sensor and calculates BPM.
	 * BPM is used for other analysis.
	 **/
	SensorTimer pulseMe(mode);
	/**
	 * startms function of Sensor timer is non blocking.
	 * The control of the main program will just whiz pass it.
	 **/
	pulseMe.startms(2);
	/**
	 * The QApplication will form the windows for the display of raw data read from the sensor.
	 * This will block the control and the program will keep on runnig stuck here.
	 **/
   	QApplication a(argc, argv);
   	SenseWindow w;
   	w.showMaximized();
   	a.exec();

	/**
	 * If the graphing window is terminated by the user then the control will get stuck in the while loop which depends on
	 * the global variable bool runnig
	 **/

	while(g_running);


	/**
	 * The timer will stop if the control reaches at pulseMe.stop
	 **/
        pulseMe.stop();


    return 0;

}//int main(int argc, char *argv[])


