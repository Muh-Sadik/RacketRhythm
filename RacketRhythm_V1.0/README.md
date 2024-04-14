# Setting up Instructions
1-Clone the Repository: 
* Clone the Racket Rhythm repository to your local machine and use the last version contained in RacketRhythm_V1.0 folder to run needed commands. 
* Note: src file under the master branch was used for development and initial testing and debugging for systems different devices.

2-Hardware Setup:
* Connect Adrafruit I2S MEMs Microphone breakout boards to Raspberry Pi GPIO pins according to provided specifications in the main README file.
* Use USB ports to connect the audio sound card then make sure to plug the speakers to the output channel of the audio sound card.
* For audio input, use any 3.5mm to 3.5mm audio cable to connect to your PC or mobilephone.

3-Software Setup:
* Configure I2S settings on Raspberry Pi by editing ```/boot/firmware/config.txt``` and ```/etc/modules```.
* Compile kernel and install necessary dependencies.

4-Library Configuration:
* Install required libraries such as Alsa, PortAudio, and FFTW.

5-Microphone Testing:
* Use ```arecord``` command to record audio and ```aplay``` command to play back the recorded file for microphone testing.

6-Adjust the ID's of PLAYBACK Hardware Devices:  
* Use ```aplay -l``` to check the list of connected hardware devices and change the below device numbers accordingly.

```https://github.com/Muh-Sadik/RacketRhythm/blob/3dd21cd3761f9f555ff87d452df89c9c807c063c/RacketRhythm_V1.0/src/main.cpp#L43```

```https://github.com/Muh-Sadik/RacketRhythm/blob/3dd21cd3761f9f555ff87d452df89c9c807c063c/RacketRhythm_V1.0/src/main.cpp#L110C1-L111C121```

7-Compile/CMake:
* Compile the RacketRhythm_V1.0 code using CMakeLists or any appropriate compiler.

8-Choose a Song:
* Select your favourite song from Spotify or YouTube.

9-Run:
* Run the compiled code to start the Racket Rhythm game experience.

# How to use CMake:
1-Create Build Directory:
* Make a directory named "Build" to hold the build files by running the command ```mkdir Build```.

2-Navigate to the Build Director:
* Go into the Build directory by running the command ```cd Build```. 

3-Run CMake Configuration:
* Generate the build files by running the command ```cmake ...```  to configure the project. This command tells CMake to look for the CMakeLists.txt file in the parent directory and generate the necessary build files based on instructions included in it.

4-Build the Project:
* Compile your project using the command ```make``` to build the project.


Make sure your CMakeLists.txt file is correctly configured to define your project's build settings and dependencies.


