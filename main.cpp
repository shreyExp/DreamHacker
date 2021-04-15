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
   // 
   // //pid_t window_pid;
   // ////Functionality for graph
   // //if(0 == (window_pid = fork())){
   // //	QApplication a(argc, argv);
   // //	SenseWindow w;
   // //	w.showMaximized();
   // //	a.exec();
   // //}

   // //pthread_t tid;
   // //pthread_attr_t attr;
   // //pthread_attr_init(&attr);
   // //pthread_create(tid, &attr, window_thread, &Signal);
   // 
   // //UNCOMMENT THE BELOW PART FOR GRAPH
   // 	//QApplication a(argc, argv);
   // 	//SenseWindow w;
   // 	//w.showMaximized();
   // 	//a.exec();

	SensorTimer pulseMe;
	pulseMe.startms(2);

	std::this_thread::sleep_for(std::chrono::seconds(1000));
        pulseMe.stop();

    //pthread_join(tid, NULL);

    return 0;

}//int main(int argc, char *argv[])


