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
#include "../include/mainwindow.h"
#include "../include/CppTimer.h"
#include <thread>
#include "../include/PulseSensor.h"
#include "../include/json_fastcgi_web_api.h"




/** class SENSORfastcgicallback
 * brief Callback class when beats data arrive
 * details Handler which receives the data here just saves
 *         the most recent sample with timestamp. Obviously,
 *         the data would be forwarde to frontend for showing
 *         to user.
 **/
class SENSORfastcgicallback : public SensorCallback {
public:
  int beatsPerMinute;
  bool sleep;
  int threshold;
  long t;

  /**
   * brief Callback with the fresh pulse data.
   * details This function is called whenever
   *         a new pulse data is arrived. It updates
   *         the local variables, which in turn forwarded
   *         to frontend.
   * param beats int -- Beats per minute, real data from sensor
   *       mayBeSleep boolean -- Sleep defining variable, true
   *                             if person is asleep.
   * return void
   **/
  virtual void hasSample(int beats, bool mayBeSleep, int bpmThreshold) {
    sleep = mayBeSleep;
    beatsPerMinute = beats;
    threshold = bpmThreshold;
    t = time(NULL);
  }

};

/** class JSONCGIADCCallback
 * brief FastCGI Callback handles
 * details Callback handler which returns data to the
 *         nginx server. Here, simply the current timestamp,
 *         sleeping possibility and the beats per minute is 
 *         transmitted to nginx and the php application.
 **/

class JSONCGIADCCallback : public JSONCGIHandler::GETCallback {
private:
  /**
   * Pointer to the ADC event handler because it keeps
   * the data in this case. 
   **/
  SENSORfastcgicallback* sensorfastcgi;

public:
  /**
   * Constructor: argument is the ADC callback handler
   * which keeps the data.
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
                 "./main [-h] for usage\n"
                 "./main [-t <int>] to put bpm threshold\n"
                 "./main [-g <bool>] 1 to plot in qtplot. Default: 1\n"
                 "./main [-l <bool>] 1 to play audio locally. Default: 1\n"
                 "./main [-n <bool>] 1 to set night time to now. Default: 0\n"
                 "./main [-w <int> ]set waiting time to confirm sleep.\n"
                 "./main [-s <bool>] 1 for simulated bpm. Default: 0\n");
}


int main(int argc, char *argv[])
{
	/**
	 * Threshold of Beats per minute
	 * Modifiable by using ./main -t <int>
	 **/
	int threshold = 77;
	/**
	 * bool value: if 1, then ecg will be plotted, if zero ecg will not be plotted
	 * Can be changed by giving arguments like this: ./main -g 0 for graph not to be plotted
	 * and ./main -g 1 for graph to be plotted. Default is 1
	 **/
	bool graph = 1;
	/**
	 * bool value: if 1, then program will run in simulation mode, if 0 then real data of bpm is used. 
	 * Can be changed by giving arguments like this: ./main -s 0 for real bpm
	 * and ./main -g 1 for simulated bpm. Default is 1
	 **/
	bool simulation = 0;

	/**
	 * bool value if 1, the audio will be played from this progrma itself. Default is 1.
	 * can be changed while running the program by giving arguments like ./main -l 0
	 **/
	bool local_audio = 1;

	/**
	 * If we want to play the audio at any time during the day then nightTime should be set to now. For that
	 * we need to set this value to 1. This can be done by passing the argument like: ./main -n 1. Default is 0
	 *
	 **/
	bool nightTimeNow = 0;

	/**
	 * int value. The delay it takes the program to reach from the state of maybesleep to sleep.
	 * Usually it must be half an our but it can be manipulated for testing purpose like this: ./main -w 60
	 * It this example the program would wait for 30 seconds to go from maybesleep state to sleep state.
	 **/
	int surelySleptTime = 2;

	bool flagSurelySleptTime = 0;

	/**
	 * This variable is used to get the parameters from the terminal
	 **/
	char key;

	/**
	 * This variable is used to get the parameters from the terminal
	 **/
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
					flagSurelySleptTime = 1;
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
	 * Pulse me runs in a c++ timer
	 * It reads the analog data from pulse sensor and calculates BPM.
	 * BPM is used for other analysis.
	 **/
	SensorTimer pulseMe(threshold, simulation, local_audio);
	if(nightTimeNow)
		pulseMe.setNigtTimeToNow();
	//waiting time just after the bpm drops to threshold
	if(flagSurelySleptTime)
		pulseMe.setSurelySleptTime(surelySleptTime);


  SENSORfastcgicallback sensorfastcgicallback;
  pulseMe.setCallback(&sensorfastcgicallback);

   /**
   * Setting up the JSONCGI communication
   * The callback which is called when fastCGI needs data
   * gets a pointer to the SENSOR callback class which
   * contains the samples. Remember this is just a simple
   * example to have access to some data.
   **/
  JSONCGIADCCallback fastCGIADCCallback(&sensorfastcgicallback);


    // starting the fastCGI handler with the callback and the
    // socket for nginx.
  JSONCGIHandler* fastCGIHandler = new JSONCGIHandler(&fastCGIADCCallback, NULL, "/tmp/sensorsocket");

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


