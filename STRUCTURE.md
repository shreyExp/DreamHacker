# Structure

This document describes the high-level folder structure of `DreamHacker project`.
If you want to familiarize yourself with the code base, you are just in the right place!

## Overview

On the highest level, `DreamHacker/drmhk_autogen` is a thing which runs in the background and gathers data and handles communication with the webserver.

It was created with the concept of event-driven logic in mind. Therefore these use events and callbacks as much as possible.

## In-detail

In this section, all paths are relative to `DreamHacker`.

### `./drmhk_autogen`

This houses the front-facing logic for `DreamHacker`. Handles CLI flags via `getopts` and sets global variables as defined. Also allows running of a test function `tests` which isn't really tests.

### `./frontend`

All the files related to developing webserver to view graphs are present in this folder.

### `./include`

All header files for the project are to be declared here and added to `CMakeLists.txt`. This allows an easy overview of what is happening within the system.

 - `CppTimer.h` -- Wrapper around C++ timer
 - `CppTimerCallback.h`  -- This class when implemented calls a function repeatedly after a several nanoseconds.
 - `json_fastcgi_web_api.h`  helps in creating communicating fastcgi socket and provides a mechanism to transfer json data to and from C++.
 - `mainwindow.h`  -- QT Graph plotting.
 - `PulseSensor.h` -- Records the Signal from the ADC, calculates beats per minute(bpm)
 		       detects sleep and plays audio.


### `./main`
Contains main.cpp file. This is the main program.

### `./src`
Contains the  `*.cpp` implementation files for `*.h` files present in the ./include directory

### `./test`
The test folder contains the test scenarios



