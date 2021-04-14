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


PulseSensor PulseSensor::instance;

//FILE* data;

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
   * Variables declared for sleep detection
   *
   **/
  time_t startOfProspectiveSleep;
  time_t surelySleptTime = 1800;
  time_t nightTime;
  time_t wakeTime;
  bool maybeSleep = 0;
  int bpmThreshold = 75;

  /**
   * Variables declared to play music
   **/
  pid_t audio_pid;
  bool is_audio_playing = 0;
  bool play_audio_locally = 0;
  char audio_name[500];
  /**
   * Variable for beats per minute simulation. For unit testing
   **/
  time_t simulation_start_time; 
  time_t simulation_has_started = 0;
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
  
  /*
   * Initialize time variables for sleep detection
   * This needs to be run at the begining of the program, for analyzeBeatsForSleep to work correctly
   */
  void initializeVariablesForSleep(void){
  	time_t current_time = time(NULL);
  	struct tm *time_split = localtime(&current_time);
  	struct tm night_time_split = {0, 0, 22, time_split->tm_mday, 
  		time_split->tm_mon, time_split->tm_year, time_split->tm_wday, time_split->tm_yday, time_split->tm_isdst};
  	nightTime = mktime(&night_time_split);
  	time_t estimatedSleepLength = 60*60*8;
  	wakeTime = nightTime + estimatedSleepLength;
  }

  /*
   * analyze beats per minute to detect sleep
   */
  bool analyzeBeatsForSleep(int bpm)
  {
      bool local_sleep = 0;
      time_t maybeSleepTime;
      /*
    * if BPM is below a certain threshold
    * mayBeSleep should be on
    * if mayBeSleep is on for a while
    * then return 1;
    */
      time_t now = time(NULL);
      if (now > nightTime && now < wakeTime) {
          if (bpm < bpmThreshold && maybeSleep == 0) {
              startOfProspectiveSleep = time(NULL);
              maybeSleep = 1;
          }
          if (bpm < bpmThreshold && maybeSleep == 1) {
              maybeSleepTime = time(NULL) - startOfProspectiveSleep;
              if (maybeSleepTime > surelySleptTime) {
                  local_sleep = 1;
              }
          }
          if (bpm > bpmThreshold && maybeSleep == 1) {
              maybeSleep == 0;
          }
      }
      else {
          local_sleep = 0;
      }
      return local_sleep;
  }



  /**
   *
   * At the start of the program set audio name must be run if the audio needs to played locally
   **/
  void set_audio_name(){
    FILE *fptr;
    if ((fptr = fopen("audio.txt", "r")) == NULL) {
        printf("Error! opening audio.txt");
    }else{
	  /**
	   * Set auddio name from audio.txt
	   **/
          fscanf(fptr,"%s", audio_name); 
          fclose(fptr);
	  play_audio_locally = 1;
    }
  }
  
  /**
   * play_audio_wrapper must be instantiated every time a bpm is read
   * This function keeps track of whether a song is already being played.
   * When the bpm reaches above the sleep threshold stops playing the audio.
   * 
   **/
  void play_audio_wrapper(){
     if(sleep == 1 && is_audio_playing == 0 && play_audio_locally){
             audio_pid = play_audio(audio_name);
             /*
              * If child process then the audio is already ended exit while loop and hence end the process.
              */
             if(audio_pid == 0){
		    //Very Important to end the program, here.
		    //End the program.
         	    //break;
             }else{
         	/*
         	 * If parent process then continue and put the is_audio_playing flag to be true
         	 */
             	is_audio_playing = 1;
             }
     }
     else if(is_audio_playing == 1 && sleep == 0 && play_audio_locally){
             /*
              * When the person wakes up again kill the pid which runs the audio
              */
             if(audio_pid > 0){
             	kill_the_pid(audio_pid);
             	is_audio_playing = 0;
             }
     }

  }

  /**
   * forks the process and plays audio, returns the pid of the process which plays the audio
   **/
  pid_t play_audio(char* audio_name){
  	pid_t local_audio_pid;
  	if(0 == (audio_pid = fork())){
  		//child process
  		execlp("mpg123", "mpg123", "-q", audio_name, 0);
  	}
  	return local_audio_pid;
  }
  
  void kill_the_pid(pid_t x){
  	char kil[50];
  	sprintf(kil,"%s%d",kil, x);
  	system(kil);
  }

  /**
   * This function needs to run if we want to generate a simulated beats per minute sequence for a sleep.
   * For unit testing
   **/
  int simulatedBeatsPerMinuteGenerator(){
	  /*
	   * 80bpm
	   *  *               *
	   *    *           *
	   *      *       *
	   *        *   *
	   *          *
	   *          60bpm
	   *    <--5 mins -->
	   */
	  if(simulation_has_started == 0)
		  //if simulation has not started yet
		  //set the simulation_start_time
		  simulation_start_time = time(NULL);

	  time_t current_time = time(NULL);

	  //Below is the variable which
	  //contains time elasped since the simulation
	  time_t t = current_time - simulation_start_time;
	  int startBPM = 80;
	  int endBPM = 60;
	  time_t duration_of_simulation = 60*5; //5 minutes
	  int slope = (endBPM - startBPM)/duration_of_simulation;
	  int local_bpm;
	  if(t <= duration_of_simulation/2)
		local_bpm = slope*t + startBPM;
	  else if(t > duration_of_simulation/2 && t < duration_of_simulation)
		local_bpm = -1 * slope*t + endBPM;
	  else local_bpm = startBPM;

	  return local_bpm;
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





