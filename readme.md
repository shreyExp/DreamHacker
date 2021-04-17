# DreamHacker

# Objectives
To give audio stimulus to a sleeping person to manipulate his dreams.

# The Plan
To take data from pulse sensor and time of the day etc, to predict when the person has slept.
Then after a while start the audio stimulus.


# Social Media

[YouTube](https://www.youtube.com/channel/UCoZ31rXYGIltQAecAKzutBQ)

[Facebook](https://www.facebook.com/Dream-Hacker-103619898510175)

[Instagram](https://www.instagram.com/proj_dreamhacker/)

# Hardware Needed
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
` cmake .` 
 `make`
4. Put your audio files inside the directory Audio/.
5. Edit the audio.txt file with the name of the audio you want to play when the sleep starts. For example if you want to play `my_file.mp3` present in the Audio/ directory then write `Audio/my_file.mp3`
7. If you want to run a simulation to test out the setup run the main function with 1 as the argument. `./main 1`. If you want to run the full program just run `./main`

# Intended UI
The UI for DreamHacker will be a Web application. By using the Raspberry Pi as a local server the user will be able to intertace with the application through any wifi enabled device.  

# Software Used 
1. PHP
1. SQL
1. Javascript
1. JQuerry


