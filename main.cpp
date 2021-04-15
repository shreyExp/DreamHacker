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
#include "json_fastcgi_web_api.h"
#include "PulseSensor.h"

struct tm* timenow;

// MCP3004/8 SETTINGS
#define BASE 100
#define SPI_CHAN 0

//Variables for sleep detection
volatile time_t startOfProspectiveSleep;
time_t surelySleptTime = 1800;
volatile time_t nightTime;
volatile time_t wakeTime;
volatile bool maybeSleep = 0;
volatile int bpmThreshold = 75;


FILE* data;

/**
 * Flag to indicate that we are running.
 * Needed later to quit the idle loop.
 **/
int mainRunning = 1;

/**
 * Handler when the user has pressed ctrl-C
 * send HUP via the kill command.
 **/
void sigHandler(int sig) { 
  if((sig == SIGHUP) || (sig == SIGINT)) {
    mainRunning = 0;
  }
}


/** 
 * Sets a signal handler so that you can kill
 * the background process gracefully with:
 * kill -HUP <PID>
 **/
void setHUPHandler() {
  struct sigaction act;
  memset (&act, 0, sizeof (act));
  act.sa_handler = sigHandler;
  if (sigaction (SIGHUP, &act, NULL) < 0) {
    perror ("sigaction");
    exit (-1);
  }
  if (sigaction (SIGINT, &act, NULL) < 0) {
    perror ("sigaction");
    exit (-1);
  }
}

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
  virtual void hasSample(int beats) {
    sleep = analyzeBeatsForSleep(beats);
    beatsPerMinute = beats;
    // timestamp
    t = time(NULL);
  }

  bool analyzeBeatsForSleep(int bpm) {
      bool sleep = 0;
      time_t maybeSleepTime;
      /*
    * if BPM is below a certain threshold
    * mayBeSleep should be on
    * if mayBeSleep is on for a while
    * then return 1;
    */
      //time_t now = time(NULL);
      //if (now > nightTime && now < wakeTime) {
      //    if (bpm < bpmThreshold && maybeSleep == 0) {
      //        startOfProspectiveSleep = time(NULL);
      //        maybeSleep = 1;
      //    }
      //    if (bpm < bpmThreshold && maybeSleep == 1) {
      //        maybeSleepTime = time(NULL) - startOfProspectiveSleep;
      //        if (maybeSleepTime > surelySleptTime) {
      //            sleep = 1;
      //        }
      //    }
      //    if (bpm > bpmThreshold && maybeSleep == 1) {
      //        maybeSleep == 0;
      //    }
      //}
      //else {
      //    sleep = 0;
      //}
      //return sleep;
      return 1;
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

int main(int argc, char* argv[]) {
    //signal(SIGINT, sigHandler);

    time_t now = time(NULL);

    wiringPiSetup(); //use the wiringPi pin numbers
    mcp3004Setup(BASE,SPI_CHAN);    // setup the mcp3004 library
    //pinMode(BLINK_LED, OUTPUT); digitalWrite(BLINK_LED,LOW);
    timenow = gmtime(&now);

    // getting all the ADC related acquistion set up
    PulseSensor* sensorcomm = new PulseSensor();
    SENSORfastcgicallback sensorfastcgicallback;
    sensorcomm->setCallback(&sensorfastcgicallback);

    // Setting up the JSONCGI communication
    // The callback which is called when fastCGI needs data
    // gets a pointer to the SENSOR callback class which
    // contains the samples. Remember this is just a simple
    // example to have access to some data.
    JSONCGIADCCallback fastCGIADCCallback(&sensorfastcgicallback);


    // starting the fastCGI handler with the callback and the
    // socket for nginx.
    JSONCGIHandler* fastCGIHandler = new JSONCGIHandler(&fastCGIADCCallback, NULL, "/tmp/sensorsocket");

    printf("starting sensor\n");
    // initilaize Pulse Sensor beat finder
    sensorcomm->initPulseSensorVariables();

    sensorcomm->startSensor();

      // catching Ctrl-C or kill -HUP so that we can terminate properly
    setHUPHandler();

    fprintf(stderr,"'%s' up and running.\n",argv[0]);

    while (mainRunning) sleep(1);


    fprintf(stderr,"'%s' shutting down.\n",argv[0]);

    // stopping ADC
    delete sensorcomm;

    // stops the fast CGI handlder
    delete fastCGIHandler;

    return 0;
  }





