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
#include <json_fastcgi_web_api.h>

/*
* min uS allowed lag btw alarm and callback
*/
#define OPT_R 10

/*
*sample time uS between alarms
*/
#define OPT_U 2000

/*
 * output option uS elapsed time between alarms
 */
#define OPT_O_ELAPSED 0

/*
 * output option uS jitter (elapsed time - sample time)
 */
#define OPT_O_JITTER 1

/*
 * default output option
 */
#define OPT_O 1

/*
 * number of samples to run (testing)
 */
#define OPT_C 10000

/*
 * number of Pulse Sensors (only 1 supported)
 */
#define OPT_N 1

#define TIME_OUT 30000000 // uS time allowed without callback response

// MCP3004/8 SETTINGS
#define BASE 100
#define SPI_CHAN 0

// FIFO STUFF
#define PULSE_EXIT 0 // CLEAN UP AND SHUT DOWN
#define PULSE_IDLE 1 // STOP SAMPLING, STAND BY
#define PULSE_ON 2 // START SAMPLING, WRITE DATA TO FILE
#define PULSE_DATA 3 // SEND DATA PACKET TO FIFO
#define PULSE_CONNECT 9 // CONNECT TO OTHER END OF PIPE

//Variables for Sleep Detection

// VARIABLES USED TO DETERMINE SAMPLE JITTER & TIME OUT
volatile unsigned int eventCounter, thisTime, lastTime, elapsedTime, jitter;
volatile int sampleFlag = 0;
volatile int sumJitter, firstTime, secondTime, duration;
unsigned int timeOutStart, dataRequestStart, m;
// VARIABLES USED TO DETERMINE BPM
volatile int Signal;
volatile unsigned int sampleCounter;
volatile int threshSetting, lastBeatTime;
volatile int thresh = 550;
volatile int P = 512; // set P default
volatile int T = 512; // set T default
volatile int firstBeat = 1; // set these to avoid noise
volatile int secondBeat = 0; // when we get the heartbeat back
volatile int QS = 0;
volatile int rate[10];
volatile int BPM = 0;
volatile int IBI = 600; // 600ms per beat = 100 Beats Per Minute (BPM)
volatile int Pulse = 0;
volatile int amp = 100; // beat amplitude 1/10 of input range.

struct tm* timenow;

//Variables for sleep detection
volatile time_t startOfProspectiveSleep;
time_t surelySleptTime = 1800;
volatile time_t nightTime;
volatile time_t wakeTime;
volatile bool maybeSleep = 0;
volatile int bpmThreshold = 75;

void getPulse(int sig_num);
void startRecording(int r, unsigned int u);
void stopTimer(void);
void initPulseSensorVariables(void);
void initJitterVariables(void);

// Function prototype for analyzing beats per minute.
bool analyzeBeatsForSleep(int);

FILE* data;

void sigHandler(int sig_num)
{
    printf("\nkilling timer\n");
    // kill the alarm
    startRecording(OPT_R, 0);
    exit(EXIT_SUCCESS);
}

/**
 * Callback handler which returns data to the
 * nginx server. Here, simply the current timestamp, sleeping possibility
 * and the beats per minute is transmitted to nginx and the
 * php application.
 **/
class JSONCGIADCCallback : public JSONCGIHandler::GETCallback {
private:
    /**
   * Variables to hold beats and sleep possibility because it keeps
   * the data in this case. 
   **/
    int beats;
    bool sleep;

public:
    /**
   * Constructor: argument is the beats per minute and sleep
   * possibility.
   **/
    JSONCGIADCCallback(int beats, bool sleep)
    {
        this->beats = beats;
        this->sleep = sleep;
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
        jsonGenerator.add("beats", beats);
        jsonGenerator.add("sleep", sleep);
        return jsonGenerator.getJSON();
    }
};

int main(int argc, char* argv[])
{
    signal(SIGINT, sigHandler);
    //int settings = 0;
    // command line settings

    //settings = initOpts(argc, argv);
    time_t now = time(NULL);

    timenow = gmtime(&now);

    //use the wiringPi pin numbers
    wiringPiSetup();

    // setup the mcp3004 library
    mcp3004Setup(BASE, SPI_CHAN);

    // initilaize Pulse Sensor beat finder
    initPulseSensorVariables();

    // start sampling
    startRecording(OPT_R, OPT_U);

    bool sleep;
    int beats;

    while (1) {
        if (sampleFlag) {
            sampleFlag = 0;
            timeOutStart = micros();
            // PRINT DATA TO TERMINAL
            printf("%lu\t%d\t%d\t%d\t%d\n", sampleCounter, Signal, BPM, IBI, jitter);
            beats = BPM;
            sleep = analyzeBeatsForSleep(beats);

            // Setting up the JSONCGI communication
            // The callback which is called when fastCGI needs data
            // gets a pointer to the SENSOR callback class which
            // contains the samples. Remember this is just a simple
            // example to have access to some data.
            JSONCGIADCCallback fastCGIADCCallback(beats, sleep);

            // starting the fastCGI handler with the callback and the
            // socket for nginx.
            JSONCGIHandler* fastCGIHandler = new JSONCGIHandler(&fastCGIADCCallback, NULL, "/tmp/fastcgi.sock");
        }
    }

    return 0;
}

bool analyzeBeatsForSleep(int bpm)
{
    bool sleep = 0;
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
                sleep = 1;
            }
        }
        if (bpm > bpmThreshold && maybeSleep == 1) {
            maybeSleep == 0;
        }
    }
    else {
        sleep = 0;
    }
    return sleep;
}

void startRecording(int r, unsigned int u)
{
    int latency = r;
    unsigned int micros = u;

    signal(SIGALRM, getPulse);
    int err = ualarm(latency, micros);
    if (err == 0) {
        if (micros > 0) {
            printf("ualarm ON\n");
        }
        else {
            printf("ualarm OFF\n");
        }
    }
}

void initPulseSensorVariables(void)
{
    for (int i = 0; i < 10; ++i) {
        rate[i] = 0;
    }
    QS = 0;
    BPM = 0;
    IBI = 600; // 600ms per beat = 100 Beats Per Minute (BPM)
    Pulse = 0;
    sampleCounter = 0;
    lastBeatTime = 0;
    P = 512; // peak at 1/2 the input range of 0..1023
    T = 512; // trough at 1/2 the input range.
    threshSetting = 550; // used to seed and reset the thresh variable
    thresh = 550; // threshold a little above the trough
    amp = 100; // beat amplitude 1/10 of input range.
    firstBeat = 1; // looking for the first beat
    secondBeat = 0; // not yet looking for the second beat in a row
    lastTime = micros();
    timeOutStart = lastTime;
}

void getPulse(int sig_num)
{

    if (sig_num == SIGALRM) {
        thisTime = micros();
        Signal = analogRead(BASE);
        elapsedTime = thisTime - lastTime;
        lastTime = thisTime;
        jitter = elapsedTime - OPT_U;
        sumJitter += jitter;
        sampleFlag = 1;

        sampleCounter += 2; // keep track of the time in mS with this variable
        int N = sampleCounter - lastBeatTime; // monitor the time since the last beat to avoid noise

        //  find the peak and trough of the pulse wave
        if (Signal < thresh && N > (IBI / 5) * 3) { // avoid dichrotic noise by waiting 3/5 of last IBI
            if (Signal < T) { // T is the trough
                T = Signal; // keep track of lowest point in pulse wave
            }
        }

        if (Signal > thresh && Signal > P) { // thresh condition helps avoid noise
            P = Signal; // P is the peak
        } // keep track of highest point in pulse wave

        //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
        // signal surges up in value every time there is a pulse
        if (N > 250) { // avoid high frequency noise
            if ((Signal > thresh) && (Pulse == 0) && (N > ((IBI / 5) * 3))) {
                Pulse = 1; // set the Pulse flag when we think there is a pulse
                IBI = sampleCounter - lastBeatTime; // measure time between beats in mS
                lastBeatTime = sampleCounter; // keep track of time for next pulse

                if (secondBeat) { // if this is the second beat, if secondBeat == TRUE
                    secondBeat = 0; // clear secondBeat flag
                    for (int i = 0; i <= 9; i++) { // seed the running total to get a realisitic BPM at startup
                        rate[i] = IBI;
                    }
                }

                if (firstBeat) { // if it's the first time we found a beat, if firstBeat == TRUE
                    firstBeat = 0; // clear firstBeat flag
                    secondBeat = 1; // set the second beat flag
                    // IBI value is unreliable so discard it
                    return;
                }

                // keep a running total of the last 10 IBI values
                int runningTotal = 0; // clear the runningTotal variable

                for (int i = 0; i <= 8; i++) { // shift data in the rate array
                    rate[i] = rate[i + 1]; // and drop the oldest IBI value
                    runningTotal += rate[i]; // add up the 9 oldest IBI values
                }

                rate[9] = IBI; // add the latest IBI to the rate array
                runningTotal += rate[9]; // add the latest IBI to runningTotal
                runningTotal /= 10; // average the last 10 IBI values
                BPM = 60000 / runningTotal; // how many beats can fit into a minute? that's BPM!
                QS = 1; // set Quantified Self flag (we detected a beat)
                //fadeLevel = MAX_FADE_LEVEL;             // If we're fading, re-light that LED.
            }
        }

        if (Signal < thresh && Pulse == 1) { // when the values are going down, the beat is over
            Pulse = 0; // reset the Pulse flag so we can do it again
            amp = P - T; // get amplitude of the pulse wave
            thresh = amp / 2 + T; // set thresh at 50% of the amplitude
            P = thresh; // reset these for next time
            T = thresh;
        }

        if (N > 2500) { // if 2.5 seconds go by without a beat
            thresh = threshSetting; // set thresh default
            P = 512; // set P default
            T = 512; // set T default
            lastBeatTime = sampleCounter; // bring the lastBeatTime up to date
            firstBeat = 1; // set these to avoid noise
            secondBeat = 0; // when we get the heartbeat back
            QS = 0;
            BPM = 0;
            IBI = 600; // 600ms per beat = 100 Beats Per Minute (BPM)
            Pulse = 0;
            amp = 100; // beat amplitude 1/10 of input range.
        }

        duration = micros() - thisTime;
    }
}
