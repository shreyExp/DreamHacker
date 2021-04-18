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




int main(int argc, char *argv[]) {
	SensorTimer pulseMe;
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

	pulseMe.startms(2);
   	QApplication a(argc, argv);
   	SenseWindow w;
   	w.showMaximized();
   	a.exec();
        pulseMe.stop();


    return 0;

}//int main(int argc, char *argv[])


