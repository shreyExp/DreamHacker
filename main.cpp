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
  int threshold
  long t;

  /**
   * Callback with the fresh ADC data.
   * That's where all the internal processing
   * of the data is happening.
   **/
  virtual void hasSample(int beats, bool mayBeSleep, int bpmThreshold) {
    sleep = mayBeSleep;
    beatsPerMinute = beats;
    threshold = bpmThreshold
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
        jsonGenerator.add("bpmThreshold", sensorfastcgi->threshold);
        return jsonGenerator.getJSON();
    }
};

volatile int g_running = 1;

void signalHandler(int signum){
	g_running = 0;
	printf("\n\n\nProgram Terminated\n\n\n");
}

void usage(void){
	printf("\nUsage:\n"
		"-h for help"
		"-t [int] to put bpm threshold\n"
		"-g [bool] 1 to plot in qtplot. Default: 1\n"
		"-l [bool] 1 to play audio locally. Default: 1\n"
		"-n [bool] 1 to set night time to now. Default: 0\n"
		"-w [int] set waiting time to confirm sleep.\n"
		"-s [bool, 0 or 1] 1 for simulated bpm. Default: 0\n");
}


int main(int argc, char *argv[])
{
	int mode = 0;
	int threshold = 77;
	bool graph = 1;
	bool simulation = 0;
	bool local_audio = 1;
	bool nightTimeNow = 0;
	int surelySleptTime = 0;
	char key;
	char* value;
	if(argc > 1){
		//mode = atoi(argv[1]);
		for(int i = 1; i < argc; i++){
			if(argv[i][0] == '-'){
				key = argv[i][1];
				value = argv[i + 1];
				switch(key){
                                case 't':
                                        threshold = atoi(value);
					i +=1;
                                        break;
                                case 'g':
                                        graph = atoi(value);
					i +=1;
                                        break;
                                case 's':
                                        simulation = atoi(value);
					i +=1;
                                        break;
                                case 'l':
                                        local_audio = atoi(value);
					i +=1;
                                        break;
                                case 'n':
                                        nightTimeNow = atoi(value);
					i +=1;
					break;
                                case 'w':
                                        surelySleptTime = atoi(value);
					i +=1;
					break;
                                case 'h':
					i +=1;
					usage();
					exit(0);
                                        break;
                                default:
					printf("Wrong Parameters passed\n\n");
					usage();
					exit(0);
                        	}

			}else{
				usage();
				exit(0);
			}
		}
	}
	signal(SIGINT, signalHandler);
	/**
	 * SensorTimer runs in a thread
	 * It reads the analog data from pulse sensor and calculates BPM.
	 * BPM is used for other analysis.
	 **/
	SensorTimer pulseMe(threshold, simulation, local_audio);
	if(nightTimeNow)
		pulseMe.setNigtTimeToNow();
	if(surelySleptTime)
		pulseMe.setSurelySleptTime(surelySleptTime);


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
	if(graph){
   		w.showMaximized();
   		a.exec();
	}

	/**
	 * If the graphing window is terminated by the user then the control will get stuck in the while loop which depends on
	 * the global variable bool runnig
	 **/

	while(g_running);


	/**
	 * The timer will stop if the control reaches at pulseMe.stop
	 **/

   //pulseMe.stop();
   pulseMe.stopNew();


    return 0;

}//int main(int argc, char *argv[])


