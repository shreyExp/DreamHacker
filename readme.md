# DreamHacker
<p align="center">
<img height=250 src="LogoDreamHacker.jpeg"  alt="logo">
</p>

Giving audio stimulus to a sleeping person to manipulate his dreams.

This setup takes data from pulse sensor and time of the day etc, to predict when the person has slept.
Then after a while start the audio stimulus.


## Social Media

[YouTube](https://www.youtube.com/channel/UCoZ31rXYGIltQAecAKzutBQ)

[Facebook](https://www.facebook.com/Dream-Hacker-103619898510175)

[Instagram](https://www.instagram.com/proj_dreamhacker/)

## Hardware Needed
1. Raspberry pi
1. Pulse Sensor
1. Wires, resistors, etc.

## Hardware Installation
 - Set up the circuit as shown in the diagram.
 - Put a speaker with it's audio jack in the audio output of the raspberry pi.

## Software Installation 
1. Use SSH, VNC or a monitor and a keyboard to login to raspberry pi.
2. Clone the repository somewhere in the Raspberry Pi.
3. Run the following commands from inside the repository 
 - `cmake .` 
 - `make`
4. Put your audio files inside the directory Audio/.
5. Edit the audio.txt file with the name of the audio you want to play when the sleep starts. For example if you want to play `my_file.mp3` present in the Audio/ directory then write `Audio/my_file.mp3`. Make sure that the file has just one line.
7. If you want to run a simulation to test out the setup run the main function with 1 as the argument. `./main 1`. If you want to run the full program just run `./main`

## User Interface
The UI for DreamHacker is a Web application on the localserver. The user will be able to monitor his heart beats per minute and set the audio which he wants to play if the audio is set to play on the server. Such audio can be listned to from a raspberry pi. 

### Software Used 
1. PHP
1. SQL
1. Javascript
1. JQuerry


