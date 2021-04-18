#include "CppTimer.h"


// MCP3004/8 SETTINGS
#define BASE 100
#define SPI_CHAN 0

/**
 * Callback for new samples which needs to be implemented by the main program.
 * The function hasSample needs to be overloaded in the main program.
 **/
class SensorCallback {
public:
	/**
	 * Called after a sample has arrived.
	 **/
	virtual void hasSample(int beats, bool mayBeSleep, int bpmThreshold) = 0;
};


class SensorTimer : public CppTimer {
	private:
    		SensorCallback* sensorCallback = nullptr;
		unsigned int eventCounter, thisTime, lastTime, elapsedTime;
		int sampleFlag = 0;
		int firstTime, secondTime, duration;
		int timeOutStart, dataRequestStart, m;
		int Signal;
		unsigned int sampleCounter;
		int threshSetting,lastBeatTime;
		int thresh = 550;
		int P = 512;                               // set P default
		int T = 512;                               // set T default
		int firstBeat = 1;                      // set these to avoid noise
		int secondBeat = 0;                    // when we get the heartbeat back
		int QS = 0;
		int rate[10];
		int BPM = 0;
		int IBI = 600;                  // 600ms per beat = 100 Beats Per Minute (BPM)
		int Pulse = 0;
		int amp = 100;                  // beat amplitude 1/10 of input range.
		int call_time_period = 2000; //in microseconds 2 milli s
		/**
		 * Variables to analyze sleep in sleep detection
		 **/
		time_t nightTime;
		time_t wakeTime;
		int bpmThreshold = 77;
		bool maybeSleep = 0;
		time_t surelySleptTime = 2;
		bool sleep;
		time_t startOfProspectiveSleep; 
		bool is_audio_playing = 0; 
		bool play_audio_locally = 1;
		pid_t audio_pid;
		char audio_name[500];
		bool is_simulation = 0;
		bool simulation_started = 0;
		time_t period_of_simulation = 60*1; //1 minute duration
	public:
		SensorTimer(int, bool, bool);
		void setCallback(SensorCallback* cb);
		void timerEvent(); 
		void initPulseSensorVariables(void);
		void getPulse(void);
		bool analyzeBeatsForSleep(int bpm);
		void initializeVariablesForSleep(void);
		void audioprocess();
		pid_t play_audio(char* audio_name);
		void kill_the_pid(pid_t x);
		void beatsPerMinuteSimulation();
		time_t start_of_simulation;
		void setPeriodOfSimulatedWave(int);
		// new funcs
		void setSurelySleptTime(int);
		void setNigtTimeToNow();
   		void stopNew();
};

SensorTimer::SensorTimer(int l_bpmThreshold, bool l_simulation, bool l_local_audio ){

	is_simulation = l_simulation;
	bpmThreshold = l_bpmThreshold;
	play_audio_locally = l_local_audio;

	initPulseSensorVariables();
	initializeVariablesForSleep();
    	FILE *fptr;
    	if ((fptr = fopen("audio.txt", "r")) == NULL) {
    	    printf("Error! opening audio.txt");
	    play_audio_locally = 0;
    	}else{
    	      fscanf(fptr,"%s", audio_name);
    	      fclose(fptr);
    	      play_audio_locally = 1;
    	}
}
void SensorTimer::setSurelySleptTime(int t){
	surelySleptTime = t;
}
void SensorTimer::setNigtTimeToNow(){
	nightTime = time(NULL) - 60;
	wakeTime = nightTime + 60*60*8;
}
void SensorTimer::setPeriodOfSimulatedWave(int period){
	period_of_simulation = period; //1 minute duration
}

void SensorTimer::stopNew(){
	stop();
}

void SensorTimer::audioprocess(){
	if(sleep == 1 && is_audio_playing == 0 && play_audio_locally){
		  is_audio_playing = 1;
		  printf("\n\n\nSleep is positive. Playing Audio.\n\n\n");
	          audio_pid = play_audio(audio_name);
	          /*
	           * If child process then the audio is already ended, pid must kill itself safely
	           */
	          if(audio_pid == 0){
	      	    raise(SIGKILL);
	          }else{
	      	/*
	      	 * If parent process then continue and put the is_audio_playing flag to be true
	      	 */
	          	is_audio_playing = 1;
			printf("\n\n\nAudio pid is %d\n\n\n", audio_pid);
	          }
	}
	else if(is_audio_playing == 1 && sleep == 0 && play_audio_locally){
	          /*
	           * When the person wakes up again kill the pid which runs the audio
	           */
	          if(audio_pid > 0){
			printf("\n\n\n Killing the Audio as maybe the person has woken up\n\n\n");
	          	kill_the_pid(audio_pid);
	          	is_audio_playing = 0;
	          }
	}
}
pid_t SensorTimer::play_audio(char* audio_name){
	pid_t audio_pid_local;
	if(0 == (audio_pid_local = fork())){
		//child process
		//printf("reached here\n");
		execlp("mpg123", "mpg123", "-q", audio_name, 0);
		is_audio_playing = 1;
	}
	is_audio_playing = 1;
	return audio_pid_local;
}

void SensorTimer::kill_the_pid(pid_t x){
	//char kil[100] = "kill -9 ";
	//sprintf(kil,"%s%d",kil, x);
	//system(kil);
	kill(x,SIGINT);
}

/**
 * Sets the callback which is called whenever there is a sample
 **/
void SensorTimer::setCallback(SensorCallback* cb) {
	//printf("pointer: %p\n", cb);
	sensorCallback = cb;
}

void SensorTimer::timerEvent() {
	getPulse();
	if(is_simulation)
		beatsPerMinuteSimulation();
	sleep = analyzeBeatsForSleep(BPM);
	audioprocess();
	printf("BPM Threshold is: %d, BPM is: %d, Maybe he is asleep: %d, "
			"Sleep is: %d, Audio_On: %d\r", bpmThreshold,  BPM, maybeSleep, sleep, is_audio_playing);
	fflush(stdout);
  if (nullptr != sensorCallback) {
      sensorCallback->hasSample(BPM, sleep, bpmThreshold);
  }
	fflush(stdout);
}

void SensorTimer::beatsPerMinuteSimulation(){
	if(simulation_started == 0){
		start_of_simulation = time(NULL);
		simulation_started = 1;
	}
	int begin_bpm = 80;
	int end_bpm = 60;
	//time_t period_of_simulation = 60*1; //5 minute duration
	float slope = (float)(end_bpm - begin_bpm)/(float)(period_of_simulation/2);
	int t = (time(NULL) - start_of_simulation)%period_of_simulation;
	//Downwards
	int sim_bpm = begin_bpm;
	//printf("slope is %f\n",slope);
	if(t < period_of_simulation/2)
		sim_bpm = (int)(slope*t) + begin_bpm;
	//Upwards
	else if(t >= period_of_simulation/2 && t <= period_of_simulation){
		t = t - (period_of_simulation/2);
		sim_bpm = -1 * (int)(slope*t) + end_bpm;
	}
	BPM = sim_bpm;

	


}

void SensorTimer::initPulseSensorVariables(void){
	
    wiringPiSetup(); //use the wiringPi pin numbers
    mcp3004Setup(BASE,SPI_CHAN);    // setup the mcp3004 library
    //pinMode(BLINK_LED, OUTPUT); digitalWrite(BLINK_LED,LOW);


    for (int i = 0; i < 10; ++i) {
        rate[i] = 0;
    }
    QS = 0;
    BPM = 0;
    IBI = 600;                  // 600ms per beat = 100 Beats Per Minute (BPM)
    Pulse = 0;
    sampleCounter = 0;
    lastBeatTime = 0;
    P = 512;                    // peak at 1/2 the input range of 0..1023
    T = 512;                    // trough at 1/2 the input range.
    threshSetting = 550;        // used to seed and reset the thresh variable
    thresh = 550;     // threshold a little above the trough
    amp = 100;                  // beat amplitude 1/10 of input range.
    firstBeat = 1;           // looking for the first beat
    secondBeat = 0;         // not yet looking for the second beat in a row
    lastTime = micros();
    timeOutStart = lastTime;
    call_time_period = 2000; //in microseconds 2 milli s
}

void SensorTimer::getPulse(void){

        thisTime = micros();
        Signal = analogRead(BASE);
        elapsedTime = thisTime - lastTime;
        lastTime = thisTime;
        sampleFlag = 1;


	  sampleCounter += 2;         // keep track of the time in mS with this variable
	  int N = sampleCounter - lastBeatTime;      // monitor the time since the last beat to avoid noise
	
	// FADE LED HERE, IF WE COULD FADE...
	
	  //  find the peak and trough of the pulse wave
	  if (Signal < thresh && N > (IBI / 5) * 3) { // avoid dichrotic noise by waiting 3/5 of last IBI
	    if (Signal < T) {                        // T is the trough
	      T = Signal;                            // keep track of lowest point in pulse wave
	    }
	  }
	
	  if (Signal > thresh && Signal > P) {       // thresh condition helps avoid noise
	    P = Signal;                              // P is the peak
	  }                                          // keep track of highest point in pulse wave
	
	  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
	  // signal surges up in value every time there is a pulse
	  if (N > 250) {                             // avoid high frequency noise
	    if ( (Signal > thresh) && (Pulse == 0) && (N > ((IBI / 5) * 3)) ) {
	      Pulse = 1;                             // set the Pulse flag when we think there is a pulse
	      IBI = sampleCounter - lastBeatTime;    // measure time between beats in mS
	      lastBeatTime = sampleCounter;          // keep track of time for next pulse
	
	      if (secondBeat) {                      // if this is the second beat, if secondBeat == TRUE
	        secondBeat = 0;                      // clear secondBeat flag
	        for (int i = 0; i <= 9; i++) {       // seed the running total to get a realisitic BPM at startup
	          rate[i] = IBI;
	        }
	      }
	
	      if (firstBeat) {                       // if it's the first time we found a beat, if firstBeat == TRUE
	        firstBeat = 0;                       // clear firstBeat flag
	        secondBeat = 1;                      // set the second beat flag
	        // IBI value is unreliable so discard it
	        return;
	      }
	
	
	      // keep a running total of the last 10 IBI values
	      int runningTotal = 0;                  // clear the runningTotal variable
	
	      for (int i = 0; i <= 8; i++) {          // shift data in the rate array
	        rate[i] = rate[i + 1];                // and drop the oldest IBI value
	        runningTotal += rate[i];              // add up the 9 oldest IBI values
	      }
	
	      rate[9] = IBI;                          // add the latest IBI to the rate array
	      runningTotal += rate[9];                // add the latest IBI to runningTotal
	      runningTotal /= 10;                     // average the last 10 IBI values
	      BPM = 60000 / runningTotal;             // how many beats can fit into a minute? that's BPM!
	      QS = 1;                              // set Quantified Self flag (we detected a beat)
	      //fadeLevel = MAX_FADE_LEVEL;             // If we're fading, re-light that LED.
	    }
	  }
	
	  if (Signal < thresh && Pulse == 1) {  // when the values are going down, the beat is over
	    Pulse = 0;                         // reset the Pulse flag so we can do it again
	    amp = P - T;                           // get amplitude of the pulse wave
	    thresh = amp / 2 + T;                  // set thresh at 50% of the amplitude
	    P = thresh;                            // reset these for next time
	    T = thresh;
	  }
	
	  if (N > 2500) {                          // if 2.5 seconds go by without a beat
	    thresh = threshSetting;                // set thresh default
	    P = 512;                               // set P default
	    T = 512;                               // set T default
	    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
	    firstBeat = 1;                      // set these to avoid noise
	    secondBeat = 0;                    // when we get the heartbeat back
	    QS = 0;
	    BPM = 0;
	    IBI = 600;                  // 600ms per beat = 100 Beats Per Minute (BPM)
	    Pulse = 0;
	    amp = 100;                  // beat amplitude 1/10 of input range.
	
	  }
	
	    duration = micros()-thisTime;
}
void SensorTimer::initializeVariablesForSleep(void){
	time_t current_time = time(NULL);
	struct tm *time_split = localtime(&current_time);
	struct tm night_time_split = {0, 0, 22, time_split->tm_mday, 
		time_split->tm_mon, time_split->tm_year, time_split->tm_wday, time_split->tm_yday, time_split->tm_isdst};
	nightTime = mktime(&night_time_split);
	if(is_simulation)
		nightTime = time(NULL) - 60;
	time_t estimatedSleepLength = 60*60*8;
	wakeTime = nightTime + estimatedSleepLength;
}

/**
* Function to analyze sleep based on beats
* per minute.
* if BPM is below a certain threshold
* mayBeSleep should be on
* if mayBeSleep is on for a while
* then return 1;
**/
bool SensorTimer::analyzeBeatsForSleep(int bpm)
{
    bool sleep_local = 0;
    time_t maybeSleepTime;
    time_t now = time(NULL);
    //printf("BPM is %d, now is %d, nightTime is %d wakeTime is %d\n", BPM, now, nightTime, wakeTime);
    if (now > nightTime && now < wakeTime) {
        if (bpm < bpmThreshold && maybeSleep == 0) {
	    printf("\n\n\nmaybesleep Positive\n\n\n");
            startOfProspectiveSleep = time(NULL);
            maybeSleep = 1;
        }
        if (bpm < bpmThreshold && maybeSleep == 1) {
            maybeSleepTime = time(NULL) - startOfProspectiveSleep;
            if (maybeSleepTime > surelySleptTime) {
                sleep_local = 1;
            }
        }
        if (bpm > bpmThreshold && maybeSleep == 1) {
            maybeSleep = 0;
        }
    }
    else {
        sleep_local = 0;
    }
    return sleep_local;
}




class SenseWindow : public MainWindow{
	private:
		int Signal;
	public:
		SenseWindow(){
		}
	void timerEvent( QTimerEvent * ) {
		Signal = analogRead(BASE);
		addRealtimeSample(Signal);
		callPlot();
	}
};
