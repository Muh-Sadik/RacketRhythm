# Racket Rhythm
Introducing Racket Rhythm: an engaging ping pong experience where gameplay consistency is driven by music. Players maintain rallies to keep the music flowing, with each hit determining the volume. Music Sound adjusts based on gameplay speed, creates a real-time system which offers a dynamic challenge for all skill levels. However consistency is king and hence, if the rally breaks down the players will miss out on listening to their favourite song. Experience the fusion of ping pong and rhythm in Racket Rhythm!

Visit our page on Instagram: [Racket Rhythm](https://www.instagram.com/racket.rhythm/)

# Key Features
* Music-driven gameplay.
* Targets amateur players improving on consistency.
* Dynamic volume adjustments for players of all levels.
* Music will stop if the rally breaks down.

# Setup Instructions
**Hardware Requirements:**
 * Raspberry Pi
 * Adrafruit I2S MEMs Microphone breakout board - SPH0645 x2
 * Audio Sound Card - AU-MMSA
 * Speakers
 * 3.5mm to 3.5mm Audio Cable

**Software Requirements:**
 * C++ for Raspberry Pi control
 
**Hardware Setup:**

Adrafruit I2S MEMs Microphone breakout board-SPH0645 was used in this setup. 
Microphone breakout board documentation can be found here: [Adafruit I2S MEMS Microphone Breakout](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-i2s-mems-microphone-breakout.pdf), and [SPH0645LM4H-B Rev C Datasheet](https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/908/SPH0645LM4H-B.pdf). GPIO pins connection are as follows:

```
Mic - RPi
---------
VCC - 3.3v
Gnd – Gnd
BCLK - BCM 18 (pin 12)
DOUT - BCM 20 (pin 38)
LRCL - BCM 19 (pin 35)
SEL  - Mono: no connection needed
     - Stero: right channel 3.3v, left channel Gnd
```

**Software Setup:**
**I2S Configuration:**  
To enable I2S on Raspberry Pi some modifications are needed:

* Uncomment ```#dtparam=i2s=on``` in ``` sudo nano /boot/firmware/config.txt ```
* Add to a new line ``` dtoverlay=i2s-mmap  ``` and ``` dtoverlay=googlevoicehat-soundcard  ``` This sets up the I2S mapping and activates the I2S sound card overlay.
* Add ``` snd-bcm2835   ``` to ``` sudo nano /etc/modules ```

**Kernel Compiling:**

* Update and install necessary dependencies:
```
$ sudo apt-get update
$ sudo rpi-update

# Check your Raspbian version:
$ gcc --version
gcc (Raspbian 12.2.0-14+rpi1) 12.2.0

# Mic dependencies that needed to be installed:
  $ sudo apt-get install bc
  $ sudo apt-get install libncurses5-dev
  ```
**Libraries Configuration:**
  
* Install Alsa library:
```
$ sudo apt install libasound2-dev
$ sudo apt-get install alsa-utils sox
 ```
* Install PortAudio library: ``` sudo apt-get install portaudio19-dev ```
* Install FFTW library: ``` sudo apt-get install libfftw3-dev ```

**Test the Microphone:**
 
* Use this command: ```arecord -f cd -r 44100 -c 1 -D plughw:1,0 -v recording.wav``` with "-v" used to display detailed information about the audio capture process or ```arecord -f cd -r 44100 -c 1 -D plughw:1,0 recording.wav``` Replace "plughw:0,0" with the appropriate device name for your setup based on the output of the aplay -l command.
* For mono system, use ```arecord -c 1 -r 48000 -f S32_LE -t wav -v filename.wav ``` or ``` Mono: arecord -f S32_LE -r 8000 -c 1 filename.wav  ```
* To play the file, use ``` Aplay filename.wav  ```
  
# Authors
 * Muhammed Sadik(2938316S)
 * Haroun Asher(2459396A)
 * Shaker Yasir(2834206M)
 * Hancheng Wang(2944209W)
 

# References

 * Alsa library
 * PortAudio library
 * FFTW  library

Clone the repository to your local machine.
```git clone https://github.com/Muh-Sadik/RacketRhythm.git```

<p align="center">
  <img src="https://github.com/Muh-Sadik/RacketRhythm/assets/157655580/689d6b9b-1b2b-4e73-951a-2b96bb608341" width="auto" height="auto">
</p>
