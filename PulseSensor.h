#ifndef __PULSESENSOR_H
#define __PULSESENSOR_H

/**
 * Copyright (c) 2021  Praveen
 **/

#include <math.h>
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
#include "CppTimer.h"

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


/**
 * Callback for new samples which needs to be implemented by the main program.
 * The function hasSample needs to be overloaded in the main program.
 **/
class SensorCallback {
public:
	/**
	 * Called after a sample has arrived.
	 **/
	virtual void hasSample(int beats) = 0;
};


/**
 * This class reads data from a fake sensor in the background
 * and calls a callback function whenever data is available.
 **/
class PulseSensor : public CppTimer  {

public:
	// Constructs a PulseSensor manager using a default configuration.
    PulseSensor() {

    }

	~PulseSensor() {
		stop();
	}

	volatile unsigned int eventCounter, thisTime, lastTime, elapsedTime, jitter;
	volatile int sampleFlag = 0;
	volatile int sumJitter, firstTime, secondTime, duration;
	unsigned int timeOutStart, dataRequestStart, m;

	// Pulse detection output variables.
    // Volatile because our pulse detection code could be called from an Interrupt
    volatile int BPM;                // int that holds raw Analog in 0. updated every call to readSensor()
    volatile int Signal;             // holds the latest incoming raw data (0..1023)
    volatile int IBI;                // int that holds the time interval (ms) between beats! Must be seeded!
    volatile bool Pulse;          // "True" when User's live heartbeat is detected. "False" when not a "live beat".
    volatile bool QS;            // The start of beat has been detected and not read by the Sketch.
    volatile int FadeLevel;          // brightness of the FadePin, in scaled PWM units. See FADE_SCALE
    volatile int threshSetting;      // used to seed and reset the thresh variable
    volatile int amp;                         // used to hold amplitude of pulse waveform, seeded (sample value)
    volatile unsigned long lastBeatTime;      // used to find IBI. Time (sampleCounter) of the previous detected beat start.

    // Variables internal to the pulse detection algorithm.
    // Not volatile because we use them only internally to the pulse detection.
    unsigned long sampleIntervalMs;  // expected time between calls to readSensor(), in milliseconds.
    int rate[10];                    // array to hold last ten IBI values (ms)
    unsigned long sampleCounter;     // used to determine pulse timing. Milliseconds since we started.
    int P;                           // used to find peak in pulse wave, seeded (sample value)
    int T;                           // used to find trough in pulse wave, seeded (sample value)
    int thresh;                      // used to find instant moment of heart beat, seeded (sample value)
    bool firstBeat;               // used to seed rate array so we startup with reasonable BPM
    bool secondBeat;              // used to seed rate array so we startup with reasonable BPM

	/**
	 * Sets the callback which is called whenever there is a sample
	 **/
	void setCallback(SensorCallback* cb) {
		printf("pointer: %p\n", cb);
		sensorCallback = cb;
	}

	/**
	 * Starts the data acquisition in the background and the
	 * callback is called with new samples
	 **/
	void startSensor() {
		printf("startSensor \n");
		startRecording(OPT_R, OPT_U);
		start(2000000);
	}

	/**
	 * Stops the data acquistion
	 **/
	void stopSensor() {
		stop();
	}

	/**
	 * Check the new data and pass to callback
	 **/
	void timerEvent() {
	printf("SIGALRM: %d\n", SIGALRM);
	getPulse(SIGALRM);
		if (sampleFlag && ( nullptr != sensorCallback) ) {
			printf("beats, pointer %d\n%p\n", BPM, sensorCallback);
            sensorCallback->hasSample(BPM);
        }

    }

    void startRecording(int r, unsigned int u) {
	    int latency = r;
	    unsigned int micros = u;

	    signal(SIGALRM, PulseSensor::static_myHandler);
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


	void initPulseSensorVariables(void) {
		printf("initPulseSensorVariables \n");
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


void getPulse(int sig_num) {
    if (sig_num == SIGALRM) {
    	//printf("Main if loop");
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
printf("beats from getPulse: %d\n", BPM);
    }
}

static void static_myHandler(int signum) {
        printf("signum: %d\n", signum);
instance.getPulse(signum);
    }


private:
	SensorCallback* sensorCallback = nullptr;
private:
	static PulseSensor instance;

};

PulseSensor PulseSensor::instance;

#endif
