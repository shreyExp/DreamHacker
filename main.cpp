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
#include "json_fastcgi_web_api.h"


/**
 * Handler which receives the data here just saves
 * the most recent sample with timestamp. Obviously,
 * in a real application the data would be stored
 * in a database and/or triggers events and other things!
 **/
class SENSORfastcgicallback : public SensorCallback {
public:
  int beatsPerMinute;
  bool sleep;
  long t;

  /**
   * Callback with the fresh ADC data.
   * That's where all the internal processing
   * of the data is happening.
   **/
  virtual void hasSample(int beats, bool mayBeSleep) {
    sleep = mayBeSleep;
    beatsPerMinute = beats;
    t = time(NULL);
  }

};

/**
 * Callback handler which returns data to the
 * nginx server. Here, simply the current timestamp, sleeping possibility
 * and the beats per minute is transmitted to nginx and the
 * php application.
 **/
class JSONCGIADCCallback : public JSONCGIHandler::GETCallback {
private:
  /**
   * Pointer to the ADC event handler because it keeps
   * the data in this case. In a proper application
   * that would be probably a database class or a
   * controller keeping it all together.
   **/
  SENSORfastcgicallback* sensorfastcgi;

public:
  /**
   * Constructor: argument is the ADC callback handler
   * which keeps the data as a simple example.
   **/
  JSONCGIADCCallback(SENSORfastcgicallback* argSENSORfastcgi) {
    sensorfastcgi = argSENSORfastcgi;
  }

    /**
   * Gets the data sends it to the webserver.
   * The callback creates three json entries. One with the
   * timestamp, one with the beats and one with the 
   * sleep possibility from the sensor.
   **/
    virtual std::string getJSONString()
    {
        JSONCGIHandler::JSONGenerator jsonGenerator;
        jsonGenerator.add("epoch", (long)time(NULL));
        jsonGenerator.add("beats", sensorfastcgi->beatsPerMinute);
        jsonGenerator.add("sleep", sensorfastcgi->sleep);
        return jsonGenerator.getJSON();
    }
};

volatile int g_running = 1;

void signalHandler(int signum){
	g_running = 0;
	printf("\n\n\nProgram Terminated\n\n\n");
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
  SENSORfastcgicallback sensorfastcgicallback;
  pulseMe.setCallback(&sensorfastcgicallback);

    // Setting up the JSONCGI communication
    // The callback which is called when fastCGI needs data
    // gets a pointer to the SENSOR callback class which
    // contains the samples. Remember this is just a simple
    // example to have access to some data.
  JSONCGIADCCallback fastCGIADCCallback(&sensorfastcgicallback);


    // starting the fastCGI handler with the callback and the
    // socket for nginx.
  JSONCGIHandler* fastCGIHandler = new JSONCGIHandler(&fastCGIADCCallback, NULL, "/tmp/sensorsocket");
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


