#! /bin/sh

#This init file is used to configure the input audio source and output audio source. The file can be edited to change the volume.

pactl set-default-sink alsa_output.usb-C-Media_Electronics_Inc._USB_Audio_Device-00.analog-stereo
pactl set-default-source alsa_input.usb-C-Media_Electronics_Inc._USB_Audio_Device-00.mono-fallback
pactl set-source-volume alsa_input.usb-C-Media_Electronics_Inc._USB_Audio_Device-00.mono-fallback 16000
pactl load-module module-loopback latency_msec=1
