<h1 align="center">Dream Hacker</h1> 
<br />
<div align="center">
   <a href="https://github.com/praveen1992/DreamHacker/">
    <img src="https://github.com/shreyExp/DreamHacker/blob/master/frontend/pictures/dreamHacker.png" alt="Logo" width="100%" height="auto">
  </a>
   <br />
   <br />
  <p align="left">
    Dream Hacker is program designed to help you have better dreams. Our projects monitors our users heart rate and plays an audio file chosen by the user. Once Dream Hacker detects the user has entered sleep ie. when the beats per minute is below a certain threshold it will play the selected audio.
    <br />
 
  <p align="center">
    This setup takes data from pulse sensor and time of the day etc, to predict when the person has slept.
    Then after a while start the audio stimulus.
    <br />
    <br />
    <a href="http://docs.dreamhacker.co.uk/"><strong>Checkout docs »</strong></a>
    <br />
    <a href="https://github.com/shreyExp/DreamHacker/wiki"><strong>Read the wiki »</strong></a>
    <br />
    <br />
    <a href="https://www.youtube.com/channel/UCoZ31rXYGIltQAecAKzutBQ"><img height=30 src="images/youtube.svg"></img></a>&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="https://www.facebook.com/Dream-Hacker-103619898510175"><img height=30 src="images/facebook.svg"></img></a>&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="https://www.instagram.com/proj_dreamhacker/"><img height=40 src="images/instagram.svg"></img></a>&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="https://www.dreamhacker.co.uk"><img height=30 src="images/LogoDreamHacker.svg"></img></a>&nbsp;&nbsp;&nbsp;&nbsp;
    <br />
    <br />
    <a href="https://github.com/shreyExp/DreamHacker/discussions">Discuss with us</a>
    ·
  </p>
</div>

<!-- TOC -->
## Contents
  <ol>
    <li>
      <a href="#about">About</a>
      <ul>
        <li><a href="#hardware">Hardware</a></li>
        <li><a href="#software">Software</a></li>
        <li><a href="#roadmap">Roadmap</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#usage">Usage</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#references">References</a></li>
  </ol>


<!-- Project descirption -->

## About
    Dream Hacker is program designed to help you have better dreams. Our projects monitors our users heart rate and plays an audio file chosen by the user. Once Dream Hacker detects the user has entered sleep ie. when the beats per minute is below a certain threshold it will play the selected audio.

Website interface enables user to select the music they want to play while they are sleeping. They can also see their different information using web Interface. 
The information will include bpm (beats per minute), the threshold and when the user has slept.

<div align="center">

[![Contributors](https://img.shields.io/badge/Contributors-4-brightgreen)](https://github.com/shreyExp/DreamHacker/network/contributors)
[![Stars](https://img.shields.io/badge/Stars-2-green)](https://github.com/shreyExp/DreamHacker/stargazers)
[![Issues](https://img.shields.io/github/issues-raw/shreyExp/DreamHacker)](https://github.com/shreyExp/DreamHacker/issues)
[![License](https://img.shields.io/github/license/shreyExp/DreamHacker)](https://github.com/shreyExp/DreamHacker/blob/master/LICENSE)

<br />
</div>

If you have
any questions about DreamHacker or want to share some information with us, please go to one of
the following places:

* [Wiki](https://github.com/shreyExp/DreamHacker/wiki)
* [FaceBook](https://www.facebook.com/Dream-Hacker-103619898510175).
* [Mail](mailto:2620969T@student.gla.ac.uk).

Before you file an issue, make sure you have read the _[known issues](https://github.com/shreyExp/DreamHacker/issues)_.

### Hardware

* [Raspberry pi ](https://www.raspberrypi.org/products/raspberry-pi-4-model-b/)
* [Pulse Sensor](https://pulsesensor.com/)
* ADC MCP3008
* Wires
* Speakers
* Breadboard

### Software

[See Documentation](https://docs.dreamhacker.co.uk/)


### Roadmap
[-] Next Software Release (v2.0) - ML of sleep detection using Python and
ML algo
* [-] User input integrated
* [-] User sleep timings integrated
* [-] Social Media Presence improvement
* [-] Pulse sensor improvements
* [-] Web-Server Improvments
* [-] Gather Training Dataset
* [-] Train sleep detection
* [-] Test on real samples


<!-- Getting Started -->

## Getting Started

### Installation
Hardware
 - Set up the circuit as shown in the [diagram](https://github.com/shreyExp/DreamHacker/wiki/RaspberryPi-Hardware-Setup).
 - Put a speaker with it's audio jack in the audio output of the raspberry pi.
 - For more information: [Hardware Wiki](https://github.com/shreyExp/DreamHacker/wiki/RaspberryPi-Hardware-Setup)

Software
```
1. Use SSH, VNC or a monitor and a keyboard to login to raspberry pi.
2. Install the audio player mpg123
3. Clone the repository somewhere in the Raspberry Pi.
4. Run the following commands from inside the repository 
 - cmake .
 - make
```

### Usage
- Put your audio files inside the directory audio/.
- Edit the audio.txt file with the name of the audio you want to play when the sleep starts. For example if you want to play `my_file.mp3` present in the audio/ directory then write `audio/my_file.mp3`. Make sure that the file has just one line.
- Run ./drmhk -h to explore what are the parameters which can be feeded as the input to the program.
- __Example:__ If you want to run the program as a simulation  and set the beats per minute threshold to 70 then run `./drmhk -s 1 -t 70`.
- To run in default mode run `./drmhk`

## Contributing

See [CONTRIBUTING](https://github.com/shreyExp/DreamHacker/wiki/Contribution) for more information.

<!-- License -->

## License

Distributed under the GPL-3.0 License. See `LICENSE` for more information.


<!-- Contact Info -->

## Contact
Team 39 in  Real Time Embedded Programming

👤 **Amber Dehariya (2594660d)**

👤 **Michael Hiluf (2529225h)**

👤 **Praveen (2620969t)**

👤 **Shreyansh Singh (2611417s)**

## References
* [jQuery <--> C++ fastcgi web API, Bernd Porr](https://github.com/berndporr/json_fastcgi_web_api)
* [Pulse Sensor](https://github.com/WorldFamousElectronics/Raspberry_Pi/blob/master/PulseSensor_C_Pi/PulseSensor_C_Pi.md)
* [CppTimer](https://github.com/berndporr/cppTimer)
* [Qt Plot](https://github.com/berndporr/qcustomplotdemo)

