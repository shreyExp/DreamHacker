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









int main(int argc, char *argv[])
{
	SensorTimer pulseMe;
	pulseMe.startms(2);
   	QApplication a(argc, argv);
   	SenseWindow w;
   	w.showMaximized();
   	a.exec();


        pulseMe.stop();


    return 0;

}//int main(int argc, char *argv[])


